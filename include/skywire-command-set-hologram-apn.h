#ifndef SKYWIRE_STEP_SET_APN_H
#define SKYWIRE_STEP_SET_APN_H

#include "skywire-command.h"

class SetApnHologramSkywireCommand : public SkywireCommand
{
public:
    SetApnHologramSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content));

    virtual SkywireResponseResult_t process() override;

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

#endif // SKYWIRE_STEP_SET_APN_H
