#ifndef SKYWIRE_STEP_ENABLE_GPS_H
#define SKYWIRE_STEP_ENABLE_GPS_H

#include "skywire-command.h"

class EnableGpsSkywireCommand : public SkywireCommand
{
public:
    EnableGpsSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content)) : SkywireCommand(skywire, "AT$GPSP=1", debug_mode, on_completed_function) {}

    bool okReceived() override
    {
        if (SkywireCommand::okReceived() || rx_buffer.indexOf("ERROR") != -1)
            return true;

        return false;
    }
};

#endif // SKYWIRE_STEP_ENABLE_GPS_H
