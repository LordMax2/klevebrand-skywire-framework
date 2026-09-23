#pragma once

#ifndef SKYWIRE_COMMAND_SET_HOLOGRAM_APN_H
#define SKYWIRE_COMMAND_SET_HOLOGRAM_APN_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize>
class SetApnHologramSkywireCommand
{
public:
    SetApnHologramSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    void reset();

    [[nodiscard]] bool completed() const { return _at.completed(); }

    [[nodiscard]] unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    [[nodiscard]] const __FlashStringHelper *getCommand() const { return _at.getCommand(); }

    [[nodiscard]] char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    enum class State
    {
        SendSet,
        WaitSet,
        SendQuery,
        WaitQuery,
        Done
    };

    SkywireAtEngine<RxBufferSize> _at;
    State _state;
};

#include "skywire-command-set-hologram-apn.ipp"

#endif
