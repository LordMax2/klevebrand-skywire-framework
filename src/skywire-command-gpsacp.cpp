#include "skywire-command-gpsacp.h"

GpsAcpSkywireCommand::GpsAcpSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT$GPSACP"), debug_mode, on_completed_function)
{
}
