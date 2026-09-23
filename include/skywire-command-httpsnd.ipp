template<size_t RxBufferSize, size_t HttpSendPathSize, size_t HttpSendPayloadSize>
HttpSndSkywireCommand<RxBufferSize, HttpSendPathSize, HttpSendPayloadSize>::HttpSndSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const char path[HttpSendPathSize],
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#HTTPSND=0,0,"), debug_mode, on_completed_function),
      _has_sent_payload(false),
      _has_received_ok(false)
{
    static_assert(SkywireCommandConcept<HttpSndSkywireCommand<RxBufferSize, HttpSendPathSize, HttpSendPayloadSize>>, "HttpSndSkywireCommand doesnt implement the concept");
    strncpy(_path, path != nullptr ? path : "", sizeof(_path) - 1);
    _path[sizeof(_path) - 1] = '\0';
    _payload[0] = '\0';
}

template<size_t RxBufferSize, size_t HttpSendPathSize, size_t HttpSendPayloadSize>
bool HttpSndSkywireCommand<RxBufferSize, HttpSendPathSize, HttpSendPayloadSize>::arrowsReceived() const
{
    return skywireContainsFlashString(SkywireAtEngine<RxBufferSize>::getRxBuffer(), PSTR(">"));
}

template<size_t RxBufferSize, size_t HttpSendPathSize, size_t HttpSendPayloadSize>
void HttpSndSkywireCommand<RxBufferSize, HttpSendPathSize, HttpSendPayloadSize>::setPayload(const char *payload_to_send)
{
    strncpy(_payload, payload_to_send != nullptr ? payload_to_send : "", sizeof(_payload) - 1);
    _payload[sizeof(_payload) - 1] = '\0';
}

template<size_t RxBufferSize, size_t HttpSendPathSize, size_t HttpSendPayloadSize>
void HttpSndSkywireCommand<RxBufferSize, HttpSendPathSize, HttpSendPayloadSize>::reset()
{
    _at.reset();
    _has_sent_payload = false;
    _has_received_ok = false;
}

template<size_t RxBufferSize, size_t HttpSendPathSize, size_t HttpSendPayloadSize>
bool HttpSndSkywireCommand<RxBufferSize, HttpSendPathSize, HttpSendPayloadSize>::okReceived() const
{
    return _has_received_ok || skywireContainsFlashString(SkywireAtEngine<RxBufferSize>::getRxBuffer(), PSTR("\r\nOK\r\n"));
}

template<size_t RxBufferSize, size_t HttpSendPathSize, size_t HttpSendPayloadSize>
SkywireResponseResult_t HttpSndSkywireCommand<RxBufferSize, HttpSendPathSize, HttpSendPayloadSize>::process()
{
    char *rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    const unsigned long now = millis();

    _at.recordFirstProcessCall();

    if (!_at.hasSent())
    {
        if (now - _at.getFirstProcessCallTimestamp() > 200)
        {
            _at.resetRxBuffer();

            if (SkywireAtEngine<RxBufferSize>::debugMode())
            {
                Serial.print(_at.getCommand());
                Serial.print(_path);
                Serial.print(F(","));
                Serial.print(strlen(_payload));
                Serial.print(F("\r"));
                Serial.println();
            }

            _at.printToModem(_at.getCommand());
            _at.printToModem(_path);
            _at.printToModem(F(","));
            _at.printToModem(static_cast<int>(strlen(_payload)));
            _at.printToModem('\r');
            _at.setSent(true);
        }

        return {false, rx_buffer};
    }

    _at.serialReadToRxBuffer();
    rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    if (okReceived() && _at.hasSent())
    {
        if (!_has_received_ok)
        {
            _at.resetRxBuffer();
            if (SkywireAtEngine<RxBufferSize>::debugMode())
            {
                Serial.println(F("CLEAR"));
            }

            _has_received_ok = true;
        }
    }
    else
    {
        return {false, rx_buffer};
    }

    if (_at.hasSent() && arrowsReceived() && !_has_sent_payload)
    {
        if (SkywireAtEngine<RxBufferSize>::debugMode())
        {
            Serial.print(F("HTTPSND Sending payload: "));
            Serial.println(_payload);
        }

        _at.printToModem(_payload);
        _at.writeToModem(0x1A);
        _has_sent_payload = true;

        if (SkywireAtEngine<RxBufferSize>::debugMode())
        {
            Serial.println(F("HTTPSND payload sent, waiting for final response"));
        }
    }

    const bool is_complete = completed();
    if (is_complete)
    {
        _at.notifyCompletedIfNeeded();
        _at.setCompleted(true);
    }

    return {is_complete, rx_buffer};
}

template<size_t RxBufferSize, size_t HttpSendPathSize, size_t HttpSendPayloadSize>
bool HttpSndSkywireCommand<RxBufferSize, HttpSendPathSize, HttpSendPayloadSize>::completed() const
{
    return _at.hasMarkedCompleted() || (_payload[0] == '\0') || (_at.hasSent() && _has_sent_payload && okReceived());
}
