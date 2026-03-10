#ifndef SKYWIRE_STEP_GPS_ACP_H
#define SKYWIRE_STEP_GPS_ACP_H

#include "Arduino.h"
#include "skywire-command.h"

class GpsAcpSkywireCommand : public SkywireCommand
{
public:
    GpsAcpSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);
};

#endif