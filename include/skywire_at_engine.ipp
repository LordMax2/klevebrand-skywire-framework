inline void logSkywireStepTimeout(
    const __FlashStringHelper *command,
    const unsigned long timeout_milliseconds,
    const unsigned long last_send_timestamp,
    const char *receive_buffer)
{
    Serial.print(F("Skywire command step: "));
    Serial.print(command);
    Serial.print(F(", after "));
    Serial.print(timeout_milliseconds);
    Serial.print(F("ms, restarting sequence. Last send timestamp: "));
    Serial.print(last_send_timestamp);
    Serial.print(F(", current timestamp: "));
    Serial.println(millis());
    Serial.print(F("rx_buffer at timeout: ["));
    Serial.print(receive_buffer);
    Serial.println(F("]"));
}

template<size_t RxBufferSize>
SkywireAtEngine<RxBufferSize>::SkywireAtEngine(
    HardwareSerial *skywire,
    const __FlashStringHelper *command,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _command(command),
      _on_completed_function(on_completed_function),
      _last_send_timestamp(0),
      _first_process_call_timestamp(0),
      _has_sent(false),
      _has_called_on_completed(false),
      _has_recorded_first_process_call(false),
      _has_marked_completed(false)
{
    static_assert(SkywireCommandConcept<SkywireAtEngine<RxBufferSize>>, "SkywireAtEngine doesnt implement the concept");

    _skywire = skywire;
    _debug_mode = debug_mode;
}

template<size_t RxBufferSize>
const __FlashStringHelper *SkywireAtEngine<RxBufferSize>::getCommand() const
{
    return _command;
}

template<size_t RxBufferSize>
bool SkywireAtEngine<RxBufferSize>::debugMode()
{
    return _debug_mode;
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::beginModem()
{
    _skywire->begin(115200);
}

template<size_t RxBufferSize>
char *SkywireAtEngine<RxBufferSize>::getRxBuffer()
{
    return _rx_buffer;
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::resetRxBuffer()
{
    _rx_buffer_cursor_index = 0;
    _rx_buffer[0] = '\0';
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::serialReadToRxBuffer()
{
    while (_skywire->available())
    {
        const char c = static_cast<char>(_skywire->read());

        if (_rx_buffer_cursor_index < sizeof(_rx_buffer) - 1)
        {
            _rx_buffer[_rx_buffer_cursor_index++] = c;
            _rx_buffer[_rx_buffer_cursor_index] = '\0';
        }
        else if (_debug_mode)
        {
            Serial.println(F("RX buffer overflow. Character not appended."));
            Serial.print(F("Size: "));
            Serial.println(sizeof(_rx_buffer) - 1);
        }

        if (_debug_mode)
        {
            const uint8_t byte_value = static_cast<uint8_t>(c);

            Serial.print(F("RX BYTE 0x"));
            if (byte_value < 0x10)
            {
                Serial.print(F("0"));
            }

            Serial.print(byte_value, HEX);
            Serial.print(F(" '"));
            if (byte_value == '\r')
            {
                Serial.print(F("CR"));
            }
            else if (byte_value == '\n')
            {
                Serial.print(F("LF"));
            }
            else if (byte_value == '\t')
            {
                Serial.print(F("TAB"));
            }
            else if (byte_value >= 32 && byte_value <= 126)
            {
                Serial.print(static_cast<char>(byte_value));
            }
            else
            {
                Serial.print(F("."));
            }

            Serial.println(F("'"));
        }
    }
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::writeCommandToModem()
{
    _skywire->print(_command);
    _skywire->print('\r');

    if (_debug_mode)
    {
        Serial.println(_command);
    }
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::printToModem(const __FlashStringHelper *value)
{
    _skywire->print(value);
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::printToModem(const char *value)
{
    _skywire->print(value);
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::printToModem(const char value)
{
    _skywire->print(value);
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::printToModem(const int value)
{
    _skywire->print(value);
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::writeToModem(const uint8_t value)
{
    _skywire->write(value);
}

template<size_t RxBufferSize>
bool SkywireAtEngine<RxBufferSize>::modemAvailable() const
{
    return _skywire->available() > 0;
}

template<size_t RxBufferSize>
bool SkywireAtEngine<RxBufferSize>::okReceived() const
{
    return skywireContainsFlashString(_rx_buffer, PSTR("\r\nOK\r\n")) ||
           skywireContainsFlashString(_rx_buffer, PSTR("+CME ERROR: context already activated"));
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::recordFirstProcessCall()
{
    if (!_has_recorded_first_process_call)
    {
        _first_process_call_timestamp = millis();
        _has_recorded_first_process_call = true;
    }
}

template<size_t RxBufferSize>
unsigned long SkywireAtEngine<RxBufferSize>::getFirstProcessCallTimestamp() const
{
    return _first_process_call_timestamp;
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::setSent(const bool has_sent)
{
    _has_sent = has_sent;

    if (has_sent)
    {
        _last_send_timestamp = millis();
    }
}

template<size_t RxBufferSize>
bool SkywireAtEngine<RxBufferSize>::hasSent() const
{
    return _has_sent;
}

template<size_t RxBufferSize>
unsigned long SkywireAtEngine<RxBufferSize>::getLastSendTimestamp() const
{
    return _last_send_timestamp;
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::setCompleted(const bool is_completed)
{
    _has_marked_completed = is_completed;
}

template<size_t RxBufferSize>
bool SkywireAtEngine<RxBufferSize>::hasMarkedCompleted() const
{
    return _has_marked_completed;
}

template<size_t RxBufferSize>
bool SkywireAtEngine<RxBufferSize>::hasCalledOnCompleted() const
{
    return _has_called_on_completed;
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::setHasCalledOnCompleted(const bool has_called_on_completed)
{
    _has_called_on_completed = has_called_on_completed;
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::notifyCompletedIfNeeded()
{
    if (_on_completed_function != nullptr && !hasCalledOnCompleted())
    {
        _on_completed_function(getRxBuffer());
        setHasCalledOnCompleted(true);
    }
}

template<size_t RxBufferSize>
bool SkywireAtEngine<RxBufferSize>::completed() const
{
    return hasMarkedCompleted() || (hasSent() && okReceived());
}

template<size_t RxBufferSize>
void SkywireAtEngine<RxBufferSize>::reset()
{
    _last_send_timestamp = 0;
    _first_process_call_timestamp = 0;
    _has_sent = false;
    _has_called_on_completed = false;
    _has_recorded_first_process_call = false;
    _has_marked_completed = false;
    resetRxBuffer();
}

template<size_t RxBufferSize>
bool SkywireAtEngine<RxBufferSize>::waitForSendThenRead()
{
    const unsigned long now = millis();

    recordFirstProcessCall();

    if (!hasSent())
    {
        if (now - getFirstProcessCallTimestamp() > 200 && getFirstProcessCallTimestamp() != 0)
        {
            resetRxBuffer();
            writeCommandToModem();
            setSent(true);
        }

        return false;
    }

    serialReadToRxBuffer();

    return true;
}

template<size_t RxBufferSize>
SkywireResponseResult_t SkywireAtEngine<RxBufferSize>::process()
{
    char *const rx_buffer = getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    if (!waitForSendThenRead())
    {
        return {false, rx_buffer};
    }

    const bool has_ok = okReceived();

    if (!has_ok)
    {
        return {false, rx_buffer};
    }

    if (_debug_mode)
    {
        Serial.print(F("STEPPER CLIENT STEP: "));
        Serial.print(_command);
        Serial.println(F(" RECEIVED OK, RX BUFFER:"));
        Serial.println(rx_buffer);
        Serial.println(F("--- END OF RX BUFFER ---"));
    }

    notifyCompletedIfNeeded();
    setCompleted(true);

    return {true, rx_buffer};
}
