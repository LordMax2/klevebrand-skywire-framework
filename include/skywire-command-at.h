#ifndef SKYWIRE_STEP_AT
#define SKYWIRE_STEP_AT

#include "skywire-command.h"

class AtSkywireCommand : public SkywireCommand
{
public:
    AtSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);
};

#endif