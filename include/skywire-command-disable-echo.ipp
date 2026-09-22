template<size_t RxBufferSize>
DisableEchoSkywireCommand<RxBufferSize>::DisableEchoSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("ATE0"), debug_mode, on_completed_function)
{
    static_assert(SkywireCommandConcept<DisableEchoSkywireCommand<RxBufferSize>>, "DisableEchoSkywireCommand doesnt implement the concept");
}
