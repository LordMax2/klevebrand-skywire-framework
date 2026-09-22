#pragma once

#ifndef SKYWIRE_COMMAND_ENABLE_GPS_H
#define SKYWIRE_COMMAND_ENABLE_GPS_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize>
class EnableGpsSkywireCommand
{
public:
    EnableGpsSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();

    void reset() { _at.reset(); }

    bool completed() const;

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *getCommand() const { return _at.getCommand(); }

    char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    bool okReceived() const;

    SkywireAtEngine<RxBufferSize> _at;
};

#include "skywire-command-enable-gps.ipp"

#endif
