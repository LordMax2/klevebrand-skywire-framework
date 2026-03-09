#include "skywire-command-gpsacp.h"

GpsAcpSkywireCommand::GpsAcpSkywireCommand(HardwareSerial *skywire, bool debug_mode, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "AT$GPSACP", debug_mode, on_completed_function)
{
}

