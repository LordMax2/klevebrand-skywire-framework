#pragma once

#ifndef SKYWIRE_COMMAND_HTTPRCV_H
#define SKYWIRE_COMMAND_HTTPRCV_H

#include "skywire_config.h"

#if SKYWIRE_ENABLE_HTTP

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

class HttpRcvSkywireCommand
{
public:
    HttpRcvSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    void reset();
    bool completed() const;
    unsigned long getSentTimestamp() const { return _at.getSentTimestamp(); }
    const __FlashStringHelper *command() const { return _at.command(); }

private:
    bool okReceived() const;

    SkywireAtEngine _at;
    unsigned long _timestamp_milliseconds;
};

static_assert(SkywireCommandConcept<HttpRcvSkywireCommand>, "HttpRcvSkywireCommand doesnt implement the concept");

#endif

#endif
