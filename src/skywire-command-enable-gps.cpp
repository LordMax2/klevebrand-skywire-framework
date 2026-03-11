#include "skywire-command-enable-gps.h"

EnableGpsSkywireCommand::EnableGpsSkywireCommand(HardwareSerial *skywire, const bool debug_mode, const OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "AT$GPSP=1", debug_mode, on_completed_function)
{
}

bool EnableGpsSkywireCommand::okReceived()
{
    auto rx_buffer = getRxBuffer();

    if (SkywireCommand::okReceived() || strstr(rx_buffer, "ERROR") != nullptr)
    {
        return true;
    }

    return false;
}
