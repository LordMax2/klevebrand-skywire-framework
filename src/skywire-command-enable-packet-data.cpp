#include "skywire-command-enable-packet-data.h"

EnablePacketDataSkywireCommand::EnablePacketDataSkywireCommand(HardwareSerial *skywire, const bool debug_mode, const OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "AT#SGACT=1,1", debug_mode, on_completed_function)
{
}

bool EnablePacketDataSkywireCommand::okReceived()
{
    const auto rx_buffer = getRxBuffer();

    if (SkywireCommand::okReceived() || strstr(rx_buffer, "ERROR") != nullptr)
    {
        return true;
    }

    return false;
}
