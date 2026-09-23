// ReSharper disable CppMissingIncludeGuard
template<size_t RxBufferSize, size_t SocketReceiveSize>
SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::SocketSendSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const __FlashStringHelper *flash_message,
    const bool should_read_response,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#SSEND=1"), debug_mode, on_completed_function),
      _flash_message(flash_message),
      _message(nullptr),
      _last_response_request_timestamp(0),
      _should_read_response(should_read_response),
      _has_sent_payload(false),
      _has_requested_response(false)
{
    static_assert(SkywireCommandConcept<SocketSendSkywireCommand>, "SocketSendSkywireCommand doesnt implement the concept");
}

template<size_t RxBufferSize, size_t SocketReceiveSize>
SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::SocketSendSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    char *message,
    const bool should_read_response,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#SSEND=1"), debug_mode, on_completed_function),
      _flash_message(nullptr),
      _message(message),
      _last_response_request_timestamp(0),
      _should_read_response(should_read_response),
      _has_sent_payload(false),
      _has_requested_response(false)
{
    static_assert(SkywireCommandConcept<SocketSendSkywireCommand>, "SocketSendSkywireCommand doesnt implement the concept");
}

template<size_t RxBufferSize, size_t SocketReceiveSize>
bool SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::hasPayload() const
{
    if (_flash_message != nullptr)
    {
        return true;
    }

    return _message != nullptr && _message[0] != '\0';
}

template<size_t RxBufferSize, size_t SocketReceiveSize>
bool SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::promptReceived() const
{
    return skywireContainsFlashString(SkywireAtEngine<RxBufferSize>::getRxBuffer(), PSTR(">"));
}

template<size_t RxBufferSize, size_t SocketReceiveSize>
bool SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::responseReceived() const
{
    char *const rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    return receivedByteCount() > 0 &&
           skywireContainsFlashString(rx_buffer, PSTR("\r\nOK\r\n"));
}

template<size_t RxBufferSize, size_t SocketReceiveSize>
int SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::receivedByteCount() const
{
    char *const marker = skywireFindFlashString(SkywireAtEngine<RxBufferSize>::getRxBuffer(), PSTR("#SRECV:"));

    if (marker == nullptr)
    {
        return -1;
    }

    const char *const byte_count = strchr(marker, ',');

    if (byte_count == nullptr)
    {
        return -1;
    }

    return atoi(byte_count + 1);
}

template<size_t RxBufferSize, size_t SocketReceiveSize>
bool SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::shouldRetrySocketRead() const
{
    if (responseRetrySuggested())
    {
        return true;
    }

    const int received_byte_count = receivedByteCount();

    if (received_byte_count > 0)
    {
        return false;
    }

    if (received_byte_count == 0)
    {
        return skywireContainsFlashString(SkywireAtEngine<RxBufferSize>::getRxBuffer(), PSTR("\r\nOK\r\n"));
    }

    return !_at.modemAvailable();
}

template<size_t RxBufferSize, size_t SocketReceiveSize>
bool SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::responseRetrySuggested() const
{
    char *const rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    return skywireContainsFlashString(rx_buffer, PSTR("SRING")) ||
           skywireContainsFlashString(rx_buffer, PSTR("+CME ERROR: operation not supported"));
}

template<size_t RxBufferSize, size_t SocketReceiveSize>
void SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::writePayloadToModem()
{
    if (_flash_message != nullptr)
    {
        _at.printToModem(_flash_message);
    }
    else if (_message != nullptr)
    {
        _at.printToModem(_message);
    }

    _at.writeToModem(0x1A);
}

template<size_t RxBufferSize, size_t SocketReceiveSize>
void SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::readSocketResponse()
{
    _at.printToModem(F("AT#SRECV=1,"));
    _at.printToModem(static_cast<int>(SocketReceiveSize));
    _at.printToModem('\r');
    _last_response_request_timestamp = millis();
    _has_requested_response = true;

    if (SkywireAtEngine<RxBufferSize>::debugMode())
    {
        Serial.print(F("AT#SRECV=1,"));
        Serial.println(static_cast<int>(SocketReceiveSize));
    }
}

template<size_t RxBufferSize, size_t SocketReceiveSize>
SkywireResponseResult_t SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::process()
{
    char *rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    _at.recordFirstProcessCall();
    const unsigned long now = millis();

    if (!_at.hasSent())
    {
        if (now - _at.getFirstProcessCallTimestamp() > 200)
        {
            _at.resetRxBuffer();
            _at.writeCommandToModem();
            _at.setSent(true);
        }

        return {false, rx_buffer};
    }

    _at.serialReadToRxBuffer();
    rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    if (!_has_sent_payload)
    {
        if (!promptReceived())
        {
            return {false, rx_buffer};
        }

        writePayloadToModem();
        _has_sent_payload = true;

        if (SkywireAtEngine<RxBufferSize>::debugMode())
        {
            Serial.print(F("Socket payload: "));
            if (_flash_message != nullptr)
            {
                Serial.println(_flash_message);
            }
            else
            {
                Serial.println(_message != nullptr ? _message : "");
            }
        }

        _at.resetRxBuffer();

        return {false, rx_buffer};
    }

    if (!_should_read_response)
    {
        if (!_at.okReceived())
        {
            return {false, rx_buffer};
        }
    }
    else
    {
        if (!responseReceived())
        {
            const bool read_attempt_settled =
                _has_requested_response &&
                now - _last_response_request_timestamp > 200;

            if (!_has_requested_response && _at.okReceived())
            {
                _at.resetRxBuffer();
                readSocketResponse();
            }
            else if (read_attempt_settled && shouldRetrySocketRead())
            {
                _at.resetRxBuffer();
                readSocketResponse();
            }

            return {false, rx_buffer};
        }
    }

    _at.notifyCompletedIfNeeded();
    _at.setCompleted(true);

    return {true, rx_buffer};
}

template<size_t RxBufferSize, size_t SocketReceiveSize>
bool SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::completed() const
{
    if (!hasPayload())
    {
        return true;
    }

    if (_should_read_response)
    {
        return _at.hasMarkedCompleted();
    }

    return _at.hasMarkedCompleted() || (_has_sent_payload && _at.okReceived());
}

template<size_t RxBufferSize, size_t SocketReceiveSize>
void SocketSendSkywireCommand<RxBufferSize, SocketReceiveSize>::reset()
{
    _at.reset();
    _last_response_request_timestamp = 0;
    _has_sent_payload = false;
    _has_requested_response = false;
}
