#pragma once

#ifndef SKYWIRE_COMMAND_HTTPSND_H
#define SKYWIRE_COMMAND_HTTPSND_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize, size_t HttpSendPathSize, size_t HttpSendPayloadSize>
class HttpSndSkywireCommand
{
public:
    HttpSndSkywireCommand(
        HardwareSerial *skywire,
        bool debug_mode,
        const char path[HttpSendPathSize],
        OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    bool completed() const;
    void reset();

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *getCommand() const { return _at.getCommand(); }

    char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

    void setPayload(const char *payload_to_send);

private:
    bool arrowsReceived() const;
    bool okReceived() const;

    SkywireAtEngine<RxBufferSize> _at;
    char _payload[HttpSendPayloadSize];
    char _path[HttpSendPathSize];
    bool _has_sent_payload;
    bool _has_received_ok;
};

#include "skywire-command-httpsnd.ipp"

#endif
