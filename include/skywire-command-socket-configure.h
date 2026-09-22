#pragma once

#ifndef SKYWIRE_COMMAND_SOCKET_CONFIGURE_H
#define SKYWIRE_COMMAND_SOCKET_CONFIGURE_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

class SocketConfigureSkywireCommand
{
public:
    SocketConfigureSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    void reset();

    bool completed() const { return _at.completed(); }

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *command() const { return _at.command(); }

private:
    enum class State
    {
        SendConfigure,
        WaitConfigure,
        SendClose,
        WaitClose
    };

    bool socketSetupFailed() const;
    bool hasReceivedSocketCloseResponse() const;

    SkywireAtEngine _at;
    State _state;
    unsigned long _recovery_started_timestamp;
};

static_assert(SkywireCommandConcept<SocketConfigureSkywireCommand>, "SocketConfigureSkywireCommand doesnt implement the concept");

#endif
