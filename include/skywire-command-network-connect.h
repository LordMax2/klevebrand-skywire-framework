#pragma once

#ifndef SKYWIRE_COMMAND_NETWORK_CONNECT_H
#define SKYWIRE_COMMAND_NETWORK_CONNECT_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize>
class NetworkConnectSkywireCommand
{
public:
    NetworkConnectSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    void reset();

    [[nodiscard]] bool completed() const { return _at.completed(); }

    [[nodiscard]] unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    [[nodiscard]] const __FlashStringHelper *getCommand() const { return _at.getCommand(); }

    [[nodiscard]] char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    static bool isNetworkConnected();

    SkywireAtEngine<RxBufferSize> _at;
    unsigned long _last_poll_timestamp;
};

#include "skywire-command-network-connect.ipp"

#endif
