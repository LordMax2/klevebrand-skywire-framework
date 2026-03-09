#ifndef SKYWIRE_STEP_AT
#define SKYWIRE_STEP_AT

#include "skywire-command.h"

class AtSkywireCommand : public SkywireCommand
{
public:
    AtSkywireCommand(HardwareSerial *skywire, bool debug_mode, void (*on_completed_function)(char* &result_content));

    virtual SkywireResponseResult_t process() override;
};

#endif