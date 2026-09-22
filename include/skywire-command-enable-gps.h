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

    const __FlashStringHelper *command() const { return _at.command(); }

    char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    bool okReceived() const;

    SkywireAtEngine<RxBufferSize> _at;
};



template<size_t RxBufferSize>
EnableGpsSkywireCommand<RxBufferSize>::EnableGpsSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT$GPSP=1"), debug_mode, on_completed_function)
{
    static_assert(SkywireCommandConcept<EnableGpsSkywireCommand<RxBufferSize>>, "EnableGpsSkywireCommand doesnt implement the concept");
}

template<size_t RxBufferSize>
bool EnableGpsSkywireCommand<RxBufferSize>::okReceived() const
{
    return _at.okReceived() || skywireContainsFlashString(SkywireAtEngine<RxBufferSize>::getRxBuffer(), PSTR("ERROR"));
}

template<size_t RxBufferSize>
SkywireResponseResult_t EnableGpsSkywireCommand<RxBufferSize>::process()
{
    char *const rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    if (!_at.waitForSendThenRead())
    {
        return {false, rx_buffer};
    }

    if (!okReceived())
    {
        return {false, rx_buffer};
    }

    _at.notifyCompletedIfNeeded();
    _at.setCompleted(true);

    return {true, rx_buffer};
}

template<size_t RxBufferSize>
bool EnableGpsSkywireCommand<RxBufferSize>::completed() const
{
    return _at.hasMarkedCompleted() || (_at.hasSent() && okReceived());
}

#endif
