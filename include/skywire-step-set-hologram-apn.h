#ifndef SKYWIRE_STEP_SET_APN_H
#define SKYWIRE_STEP_SET_APN_H

#include "skywire-step.h"

class SetApnHologramSkywireStep : public SkywireStep {
public:
    SetApnHologramSkywireStep(Skywire &skywire, bool debug_mode, void (*on_completed_function)(String& result_content)) : SkywireStep(skywire, "AT+CGDCONT=1,\"IPV4V6\",\"hologram\"\r", debug_mode, on_completed_function) {}
};

#endif // SKYWIRE_STEP_SET_APN_H
