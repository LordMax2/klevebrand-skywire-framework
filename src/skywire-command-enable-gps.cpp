#include "skywire-command-enable-gps.h"

EnableGpsSkywireCommand::EnableGpsSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "AT$GPSP=1", debug_mode, on_completed_function)
{
}

bool EnableGpsSkywireCommand::okReceived()
{
    if (SkywireCommand::okReceived() || rx_buffer.indexOf("ERROR") != -1)
    {
        return true;
    }

    return false;
}
