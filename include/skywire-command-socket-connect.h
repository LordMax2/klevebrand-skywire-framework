#pragma once

#ifndef SKYWIRE_COMMAND_SOCKET_CONNECT_H
#define SKYWIRE_COMMAND_SOCKET_CONNECT_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize>
class SocketConnectSkywireCommand
{
public:
    SocketConnectSkywireCommand(
        HardwareSerial *skywire,
        const char *host,
        int port,
        bool debug_mode,
        OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();

    void reset() { _at.reset(); }

    [[nodiscard]] bool completed() const { return _at.completed(); }

    [[nodiscard]] unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    [[nodiscard]] const __FlashStringHelper *getCommand() const { return _at.getCommand(); }

    [[nodiscard]] char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    void writeConnectCommandToModem();

    SkywireAtEngine<RxBufferSize> _at;
    const char *_host;
    int _port;
};

#include "skywire-command-socket-connect.ipp"

#endif
