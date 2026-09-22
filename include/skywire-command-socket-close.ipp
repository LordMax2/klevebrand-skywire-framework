template<size_t RxBufferSize>
SocketCloseSkywireCommand<RxBufferSize>::SocketCloseSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#SH=1"), debug_mode, on_completed_function)
{
    static_assert(SkywireCommandConcept<SocketCloseSkywireCommand<RxBufferSize>>, "SocketCloseSkywireCommand doesnt implement the concept");
}
