// ReSharper disable CppMissingIncludeGuard
template<size_t RxBufferSize>
EnablePacketDataSkywireCommand<RxBufferSize>::EnablePacketDataSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#SGACT=1,1"), debug_mode, on_completed_function)
{
    static_assert(SkywireCommandConcept<EnablePacketDataSkywireCommand>, "EnablePacketDataSkywireCommand doesnt implement the concept");
}

template<size_t RxBufferSize>
bool EnablePacketDataSkywireCommand<RxBufferSize>::okReceived() const
{
    char *const rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    return _at.okReceived() ||
           skywireContainsFlashString(rx_buffer, PSTR("ERROR")) ||
           skywireContainsFlashString(rx_buffer, PSTR("+CME ERROR: context already activated"));
}

template<size_t RxBufferSize>
SkywireResponseResult_t EnablePacketDataSkywireCommand<RxBufferSize>::process()
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
bool EnablePacketDataSkywireCommand<RxBufferSize>::completed() const
{
    return _at.hasMarkedCompleted() || (_at.hasSent() && okReceived());
}
