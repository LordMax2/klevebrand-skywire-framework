#ifndef STEP_DISABLE_ECHO_H
#define STEP_DISABLE_ECHO_H

#include "skywire-step.h"

class DisableEchoSkywireStep : public SkywireStep
{
public:
	DisableEchoSkywireStep(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content)) : SkywireStep(skywire, "ATE0", debug_mode, on_completed_function) {}
};

#endif // STEP_DISABLE_ECHO_H