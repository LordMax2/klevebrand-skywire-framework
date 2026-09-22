#pragma once

#ifndef SKYWIRE_COMMAND_DISABLE_ECHO_H
#define SKYWIRE_COMMAND_DISABLE_ECHO_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

class DisableEchoSkywireCommand
{
public:
    DisableEchoSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process() { return _at.process(); }

    void reset() { _at.reset(); }

    bool completed() const { return _at.completed(); }

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *command() const { return _at.command(); }

private:
    SkywireAtEngine _at;
};

static_assert(SkywireCommandConcept<DisableEchoSkywireCommand>, "DisableEchoSkywireCommand doesnt implement the concept");

#endif
