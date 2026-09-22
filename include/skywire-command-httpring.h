#pragma once

#ifndef SKYWIRE_COMMAND_HTTPRING_H
#define SKYWIRE_COMMAND_HTTPRING_H

#include "skywire_config.h"

#if SKYWIRE_ENABLE_HTTP

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

class HttpRingSkywireCommand
{
public:
    HttpRingSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    bool completed() const;

    void reset() { _at.reset(); }

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *command() const { return _at.command(); }

private:
    bool okReceived() const;

    SkywireAtEngine _at;
};

static_assert(SkywireCommandConcept<HttpRingSkywireCommand>, "HttpRingSkywireCommand doesnt implement the concept");

#endif

#endif
