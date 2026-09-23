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

    [[nodiscard]] bool completed() const { return _at.completed(); }

    [[nodiscard]] unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    [[nodiscard]] const __FlashStringHelper *getCommand() const { return _at.getCommand(); }

    [[nodiscard]] char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    SkywireAtEngine<RxBufferSize> _at;
};

#include "skywire-command-gpsacp.ipp"

#endif
