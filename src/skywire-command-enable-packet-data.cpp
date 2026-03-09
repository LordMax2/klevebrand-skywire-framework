#include "skywire-command-enable-packet-data.h"

EnablePacketDataSkywireCommand::EnablePacketDataSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "AT#SGACT=1,1", debug_mode, on_completed_function)
{
}

bool EnablePacketDataSkywireCommand::okReceived()
{
    if (SkywireCommand::okReceived() || _rx_buffer.indexOf("ERROR") != -1)
    {
        return true;
    }

    return false;
}
