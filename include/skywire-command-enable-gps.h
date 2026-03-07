#ifndef SKYWIRE_STEP_ENABLE_GPS_H
#define SKYWIRE_STEP_ENABLE_GPS_H

#include "skywire-command.h"

class EnableGpsSkywireCommand : public SkywireCommand
{
public:
    EnableGpsSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content));

    bool okReceived() override;
};

#endif 
