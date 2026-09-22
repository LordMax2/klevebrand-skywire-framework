#pragma once

#ifndef SKYWIRE_COMMAND_HTTPSND_H
#define SKYWIRE_COMMAND_HTTPSND_H

#include "skywire_config.h"

#if SKYWIRE_ENABLE_HTTP

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

class HttpSndSkywireCommand
{
public:
    HttpSndSkywireCommand(
        HardwareSerial *skywire,
        bool debug_mode,
        const char path[HTTP_SND_PATH_SIZE],
        OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    bool completed() const;
    void reset();

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *command() const { return _at.command(); }

    void setPayload(const char *payload_to_send);

private:
    bool arrowsReceived() const;
    bool okReceived() const;

    SkywireAtEngine _at;
    char _payload[HTTP_SND_PAYLOAD_TO_SEND_SIZE];
    char _path[HTTP_SND_PATH_SIZE];
    bool _has_sent_payload;
    bool _has_received_ok;
};

static_assert(SkywireCommandConcept<HttpSndSkywireCommand>, "HttpSndSkywireCommand doesnt implement the concept");

#endif

#endif
