#pragma once

#ifndef SKYWIRE_COMMAND_SOCKET_CLOSE_H
#define SKYWIRE_COMMAND_SOCKET_CLOSE_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

class SocketCloseSkywireCommand
{
public:
    SocketCloseSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process() { return _at.process(); }
    void reset() { _at.reset(); }
    bool completed() const { return _at.completed(); }
    unsigned long getSentTimestamp() const { return _at.getSentTimestamp(); }
    const __FlashStringHelper *command() const { return _at.command(); }

private:
    SkywireAtEngine _at;
};

static_assert(SkywireCommandConcept<SocketCloseSkywireCommand>, "SocketCloseSkywireCommand doesnt implement the concept");

#endif
