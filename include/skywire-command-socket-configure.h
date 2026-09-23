#pragma once

#ifndef SKYWIRE_COMMAND_SOCKET_CONFIGURE_H
#define SKYWIRE_COMMAND_SOCKET_CONFIGURE_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize>
class SocketConfigureSkywireCommand
{
public:
    SocketConfigureSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    void reset();

    [[nodiscard]] bool completed() const { return _at.completed(); }

    [[nodiscard]] unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    [[nodiscard]] const __FlashStringHelper *getCommand() const { return _at.getCommand(); }

    [[nodiscard]] char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    enum class State
    {
        SendConfigure,
        WaitConfigure,
        SendClose,
        WaitClose
    };

    [[nodiscard]] bool socketSetupFailed() const;
    [[nodiscard]] bool hasReceivedSocketCloseResponse() const;

    SkywireAtEngine<RxBufferSize> _at;
    State _state;
    unsigned long _recovery_started_timestamp;
};

#include "skywire-command-socket-configure.ipp"

#endif
