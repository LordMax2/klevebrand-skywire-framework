#ifndef STEP_AT
#define STEP_AT

#include "skywire-step.h"

class AtSkywireStep : public SkywireStep {
public:
    AtSkywireStep(Skywire &skywire, bool debug_mode, void (*on_completed_function)(String &result_content)) : SkywireStep(skywire, "AT", debug_mode, on_completed_function) {}
};

#endif // STEP_AT