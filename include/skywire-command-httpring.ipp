template<size_t RxBufferSize>
HttpRingSkywireCommand<RxBufferSize>::HttpRingSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("HTTPRING"), debug_mode, on_completed_function)
{
    static_assert(SkywireCommandConcept<HttpRingSkywireCommand<RxBufferSize>>, "HttpRingSkywireCommand doesnt implement the concept");
}

template<size_t RxBufferSize>
SkywireResponseResult_t HttpRingSkywireCommand<RxBufferSize>::process()
{
    char *const rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    _at.recordFirstProcessCall();

    if (!_at.hasSent())
    {
        _at.resetRxBuffer();
        _at.setSent(true);
    }

    _at.serialReadToRxBuffer();

    const bool has_ok = okReceived();
    if (SkywireAtEngine<RxBufferSize>::debugMode() && has_ok)
    {
        Serial.println(F("STEPPER CLIENT RECEIVED HTTPRING OK: "));
        Serial.println(rx_buffer);
        Serial.println(F("--- END OF RX BUFFER ---"));
    }

    if (completed())
    {
        _at.notifyCompletedIfNeeded();
        _at.setCompleted(true);
    }

    return {false, rx_buffer};
}

template<size_t RxBufferSize>
bool HttpRingSkywireCommand<RxBufferSize>::okReceived() const
{
    char *const ring = skywireFindFlashString(SkywireAtEngine<RxBufferSize>::getRxBuffer(), PSTR("HTTPRING"));
    if (ring == nullptr)
    {
        return false;
    }

    return strchr(ring, '\r') != nullptr;
}

template<size_t RxBufferSize>
bool HttpRingSkywireCommand<RxBufferSize>::completed() const
{
    return _at.hasMarkedCompleted() ||
           (_at.hasSent() && okReceived() && millis() - _at.getLastSendTimestamp() > 500);
}
