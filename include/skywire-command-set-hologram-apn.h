#ifndef SKYWIRE_STEP_SET_APN_H
#define SKYWIRE_STEP_SET_APN_H

#include "skywire-command.h"

class SetApnHologramSkywireCommand : public SkywireCommand
{
public:
    SetApnHologramSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    virtual SkywireResponseResult_t process() override;
    virtual void reset() override;

private:
    enum class State
    {
        SEND_SET,
        WAIT_SET,
        SEND_QUERY,
        WAIT_QUERY,
        DONE
    };
    State state;
};

#endif
