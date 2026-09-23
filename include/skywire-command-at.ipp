// ReSharper disable CppMissingIncludeGuard
template<size_t RxBufferSize>
AtSkywireCommand<RxBufferSize>::AtSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT"), debug_mode, on_completed_function)
{
    static_assert(SkywireCommandConcept<AtSkywireCommand>, "AtSkywireCommand doesnt implement the concept");
}
