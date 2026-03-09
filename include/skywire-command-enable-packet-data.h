#ifndef SKYWIRE_STEP_ENABLE_PACKET_DATA_H
#define SKYWIRE_STEP_ENABLE_PACKET_DATA_H

#include "skywire-command.h"

class EnablePacketDataSkywireCommand : public SkywireCommand
{
public:
    EnablePacketDataSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function) ;

    bool okReceived() override;
};

#endif
