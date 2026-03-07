#ifndef SKYWIRE_STEP_ENABLE_PACKET_DATA_H
#define SKYWIRE_STEP_ENABLE_PACKET_DATA_H

#include "skywire-command.h"

class EnablePacketDataSkywireCommand : public SkywireCommand
{
public:
    EnablePacketDataSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content));

    bool okReceived() override;
};

#endif // SKYWIRE_STEP_ENABLE_PACKET_DATA_H
