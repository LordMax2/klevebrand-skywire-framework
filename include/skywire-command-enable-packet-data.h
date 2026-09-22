#pragma once

#ifndef SKYWIRE_COMMAND_ENABLE_PACKET_DATA_H
#define SKYWIRE_COMMAND_ENABLE_PACKET_DATA_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize>
class EnablePacketDataSkywireCommand
{
public:
    EnablePacketDataSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

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



template<size_t RxBufferSize>
EnablePacketDataSkywireCommand<RxBufferSize>::EnablePacketDataSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#SGACT=1,1"), debug_mode, on_completed_function)
{
    static_assert(SkywireCommandConcept<EnablePacketDataSkywireCommand<RxBufferSize>>, "EnablePacketDataSkywireCommand doesnt implement the concept");
}

template<size_t RxBufferSize>
bool EnablePacketDataSkywireCommand<RxBufferSize>::okReceived() const
{
    char *const rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    return _at.okReceived() ||
           skywireContainsFlashString(rx_buffer, PSTR("ERROR")) ||
           skywireContainsFlashString(rx_buffer, PSTR("+CME ERROR: context already activated"));
}

template<size_t RxBufferSize>
SkywireResponseResult_t EnablePacketDataSkywireCommand<RxBufferSize>::process()
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
bool EnablePacketDataSkywireCommand<RxBufferSize>::completed() const
{
    return _at.hasMarkedCompleted() || (_at.hasSent() && okReceived());
}

#endif
