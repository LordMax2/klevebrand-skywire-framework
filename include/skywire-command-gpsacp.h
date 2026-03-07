#ifndef SKYWIRE_STEP_GPS_ACP_H
#define SKYWIRE_STEP_GPS_ACP_H

#include "Arduino.h"
#include "skywire-command.h"

class GpsAcpSkywireCommand : public SkywireCommand
{
public:
    GpsAcpSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String& result_content));
    SkywireResponseResult_t process() override;
};

#endif // SKYWIRE_STEP_GPS_ACP_H 