#pragma once

#ifndef SKYWIRE_COMMAND_NETWORK_CONNECT_H
#define SKYWIRE_COMMAND_NETWORK_CONNECT_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

class NetworkConnectSkywireCommand
{
public:
    NetworkConnectSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    void reset();
    bool completed() const { return _at.completed(); }
    unsigned long getSentTimestamp() const { return _at.getSentTimestamp(); }
    const __FlashStringHelper *command() const { return _at.command(); }

private:
    static bool isNetworkConnected();

    SkywireAtEngine _at;
    unsigned long _last_poll_timestamp;
};

static_assert(SkywireCommandConcept<NetworkConnectSkywireCommand>, "NetworkConnectSkywireCommand doesnt implement the concept");

#endif
