#pragma once

#ifndef SKYWIRE_COMMAND_ENABLE_GPS_H
#define SKYWIRE_COMMAND_ENABLE_GPS_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

class EnableGpsSkywireCommand
{
public:
    EnableGpsSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();

    void reset() { _at.reset(); }

    bool completed() const;

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *command() const { return _at.command(); }

private:
    bool okReceived() const;

    SkywireAtEngine _at;
};

static_assert(SkywireCommandConcept<EnableGpsSkywireCommand>, "EnableGpsSkywireCommand doesnt implement the concept");

#endif
