#pragma once

#ifndef SKYWIRE_COMMAND_HTTPRING_H
#define SKYWIRE_COMMAND_HTTPRING_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize>
class HttpRingSkywireCommand
{
public:
    HttpRingSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    bool completed() const;

    void reset() { _at.reset(); }

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *getCommand() const { return _at.getCommand(); }

    char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    bool okReceived() const;

    SkywireAtEngine<RxBufferSize> _at;
};

#include "skywire-command-httpring.ipp"

#endif
