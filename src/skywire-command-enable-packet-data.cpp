#include "skywire-command-enable-packet-data.h"

EnablePacketDataSkywireCommand::EnablePacketDataSkywireCommand(HardwareSerial *skywire, bool debug_mode, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "AT#SGACT=1,1", debug_mode, on_completed_function)
{
}

bool EnablePacketDataSkywireCommand::okReceived()
{
    auto rx_buffer = getRxBuffer();

    if (SkywireCommand::okReceived() || rx_buffer.indexOf("ERROR") != -1)
    {
        return true;
    }

    return false;
}
