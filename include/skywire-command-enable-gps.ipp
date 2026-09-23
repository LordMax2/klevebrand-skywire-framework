// ReSharper disable CppMissingIncludeGuard
template<size_t RxBufferSize>
EnableGpsSkywireCommand<RxBufferSize>::EnableGpsSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT$GPSP=1"), debug_mode, on_completed_function)
{
    static_assert(SkywireCommandConcept<EnableGpsSkywireCommand>, "EnableGpsSkywireCommand doesnt implement the concept");
}

template<size_t RxBufferSize>
bool EnableGpsSkywireCommand<RxBufferSize>::okReceived() const
{
    return _at.okReceived() || skywireContainsFlashString(SkywireAtEngine<RxBufferSize>::getRxBuffer(), PSTR("ERROR"));
}

template<size_t RxBufferSize>
SkywireResponseResult_t EnableGpsSkywireCommand<RxBufferSize>::process()
{
    char *const rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    if (!_at.waitForSendThenRead())
    {
        return {false, rx_buffer};
    }

    if (!okReceived())
    {
        return {false, rx_buffer};
    }

    _at.notifyCompletedIfNeeded();
    _at.setCompleted(true);

    return {true, rx_buffer};
}

template<size_t RxBufferSize>
bool EnableGpsSkywireCommand<RxBufferSize>::completed() const
{
    return _at.hasMarkedCompleted() || (_at.hasSent() && okReceived());
}
