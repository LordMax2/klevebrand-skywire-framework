#ifndef SKYWIRE_STEP_ENABLE_PACKET_DATA_H
#define SKYWIRE_STEP_ENABLE_PACKET_DATA_H

#include "skywire-command.h"

class EnablePacketDataSkywireCommand : public SkywireCommand
{
public:
    EnablePacketDataSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content)) : SkywireCommand(skywire, "AT#SGACT=1,1", debug_mode, on_completed_function) {}

    bool okReceived() override
    {
        if (SkywireCommand::okReceived() || rx_buffer.indexOf("ERROR") != -1)
            return true;

        return false;
    }
};

#endif // SKYWIRE_STEP_ENABLE_PACKET_DATA_H
