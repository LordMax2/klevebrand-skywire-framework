#pragma once

#ifndef SKYWIRE_COMMAND_SOCKET_CLOSE_H
#define SKYWIRE_COMMAND_SOCKET_CLOSE_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize>
class SocketCloseSkywireCommand
{
public:
    SocketCloseSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process() { return _at.process(); }

    void reset() { _at.reset(); }

    bool completed() const { return _at.completed(); }

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *getCommand() const { return _at.getCommand(); }

    char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    SkywireAtEngine<RxBufferSize> _at;
};

#include "skywire-command-socket-close.ipp"

#endif
