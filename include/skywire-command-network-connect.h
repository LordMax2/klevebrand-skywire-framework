#ifndef SKYWIRE_STEP_NETWORK_CONNECT_H
#define SKYWIRE_STEP_NETWORK_CONNECT_H

#include "Arduino.h"
#include "skywire-command.h"

class NetworkConnectSkywireCommand : public SkywireCommand
{
public:
    NetworkConnectSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content));

    SkywireResponseResult_t process() override;

private:
    bool isNetworkConnected();
};

#endif 
