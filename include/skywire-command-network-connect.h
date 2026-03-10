#ifndef SKYWIRE_STEP_NETWORK_CONNECT_H
#define SKYWIRE_STEP_NETWORK_CONNECT_H

#include "Arduino.h"
#include "skywire-command.h"

class NetworkConnectSkywireCommand : public SkywireCommand
{
public:
    NetworkConnectSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process() override;

private:
    bool isNetworkConnected();
};

#endif
