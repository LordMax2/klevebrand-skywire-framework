#ifndef SKYWIRE_STEP_DISABLE_ECHO_H
#define SKYWIRE_STEP_DISABLE_ECHO_H

#include "skywire-command.h"

class DisableEchoSkywireCommand : public SkywireCommand
{
public:
	DisableEchoSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content)) : SkywireCommand(skywire, "ATE0", debug_mode, on_completed_function) {}
};

#endif // SKYWIRE_STEP_DISABLE_ECHO_H