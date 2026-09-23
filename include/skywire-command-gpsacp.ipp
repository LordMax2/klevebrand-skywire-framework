// ReSharper disable CppMissingIncludeGuard
template<size_t RxBufferSize>
GpsAcpSkywireCommand<RxBufferSize>::GpsAcpSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT$GPSACP"), debug_mode, on_completed_function)
{
    static_assert(SkywireCommandConcept<GpsAcpSkywireCommand>, "GpsAcpSkywireCommand doesnt implement the concept");
}
