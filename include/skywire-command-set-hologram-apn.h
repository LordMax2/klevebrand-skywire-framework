#pragma once

#ifndef SKYWIRE_COMMAND_SET_HOLOGRAM_APN_H
#define SKYWIRE_COMMAND_SET_HOLOGRAM_APN_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

class SetApnHologramSkywireCommand
{
public:
    SetApnHologramSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    void reset();
    bool completed() const { return _at.completed(); }
    unsigned long getSentTimestamp() const { return _at.getSentTimestamp(); }
    const __FlashStringHelper *command() const { return _at.command(); }

private:
    enum class State
    {
        SendSet,
        WaitSet,
        SendQuery,
        WaitQuery,
        Done
    };

    SkywireAtEngine _at;
    State _state;
};

static_assert(SkywireCommandConcept<SetApnHologramSkywireCommand>, "SetApnHologramSkywireCommand doesnt implement the concept");

#endif
