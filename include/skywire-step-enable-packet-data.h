#ifndef SKYWIRE_STEP_ENABLE_PACKET_DATA_H
#define SKYWIRE_STEP_ENABLE_PACKET_DATA_H

#include "skywire-step.h"

class EnablePacketDataSkywireStep : public SkywireStep {
public:
    EnablePacketDataSkywireStep(Skywire &skywire, bool debug_mode, void (*on_completed_function)(String& result_content)) : SkywireStep(skywire, "AT#SGACT=1,1", debug_mode, on_completed_function) {}
};

#endif // SKYWIRE_STEP_ENABLE_PACKET_DATA_H
