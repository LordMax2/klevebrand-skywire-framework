#pragma once

#ifndef SKYWIRE_COMMAND_GPSACP_H
#define SKYWIRE_COMMAND_GPSACP_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize>
class GpsAcpSkywireCommand
{
public:
    GpsAcpSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process() { return _at.process(); }

    void reset() { _at.reset(); }

    bool completed() const { return _at.completed(); }

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *command() const { return _at.command(); }

    char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    SkywireAtEngine<RxBufferSize> _at;
};



template<size_t RxBufferSize>
GpsAcpSkywireCommand<RxBufferSize>::GpsAcpSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT$GPSACP"), debug_mode, on_completed_function)
{
    static_assert(SkywireCommandConcept<GpsAcpSkywireCommand<RxBufferSize>>, "GpsAcpSkywireCommand doesnt implement the concept");
}

#endif
