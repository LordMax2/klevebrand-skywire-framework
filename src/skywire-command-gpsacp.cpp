#include "skywire-command-gpsacp.h"

GpsAcpSkywireCommand::GpsAcpSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "AT$GPSACP", debug_mode, on_completed_function)
{
}
