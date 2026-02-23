#ifndef SKYWIRE_STEP_ENABLE_GPS_H
#define SKYWIRE_STEP_ENABLE_GPS_H

#include "skywire-step.h"

class EnableGpsSkywireStep : public SkywireStep
{
public:
    EnableGpsSkywireStep(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content)) : SkywireStep(skywire, "AT$GPSP=1", debug_mode, on_completed_function) {}

    bool okReceived() override
    {
        if (SkywireStep::okReceived() || rx_buffer.indexOf("ERROR") != -1)
            return true;

        return false;
    }
};

#endif // SKYWIRE_STEP_ENABLE_GPS_H
