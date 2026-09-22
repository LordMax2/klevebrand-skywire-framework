#pragma once

#ifndef SKYWIRE_COMMAND_SOCKET_SEND_H
#define SKYWIRE_COMMAND_SOCKET_SEND_H

#include "skywire_config.h"
#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

class SocketSendSkywireCommand
{
public:
    SocketSendSkywireCommand(
        HardwareSerial *skywire,
        bool debug_mode,
        const __FlashStringHelper *flash_message,
        bool should_read_response,
        OnCompletedFunction on_completed_function);

    SocketSendSkywireCommand(
        HardwareSerial *skywire,
        bool debug_mode,
        char *message,
        bool should_read_response,
        OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    bool completed() const;
    void reset();

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *command() const { return _at.command(); }

private:
    bool promptReceived() const;
    bool responseReceived() const;
    bool responseRetrySuggested() const;
    void writePayloadToModem();
    void readSocketResponse();
    bool hasPayload() const;

    SkywireAtEngine _at;
    const __FlashStringHelper *_flash_message;
    char *_message;
    unsigned long _last_response_request_timestamp;
    bool _should_read_response;
    bool _has_sent_payload;
    bool _has_requested_response;
};

static_assert(SkywireCommandConcept<SocketSendSkywireCommand>, "SocketSendSkywireCommand doesnt implement the concept");

#endif
