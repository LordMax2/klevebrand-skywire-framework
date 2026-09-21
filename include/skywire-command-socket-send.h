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
        bool read_response,
        OnCompletedFunction on_completed_function);

    SocketSendSkywireCommand(
        HardwareSerial *skywire,
        bool debug_mode,
        char *message,
        bool read_response,
        OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    bool completed() const;
    void reset();
    unsigned long getSentTimestamp() const { return _at.getSentTimestamp(); }
    const __FlashStringHelper *command() const { return _at.command(); }

private:
    static constexpr uint8_t kFlagReadResponse = 1 << 0;
    static constexpr uint8_t kFlagPayloadSent = 1 << 1;
    static constexpr uint8_t kFlagResponseRequested = 1 << 2;
    static constexpr uint8_t kFlagResponseReceived = 1 << 3;

    bool flag(uint8_t mask) const;
    void setFlag(uint8_t mask, bool value);
    bool promptReceived() const;
    bool responseReceived() const;
    bool responseRetrySuggested() const;
    void writePayloadToModem();
    void readSocketResponse();
    bool hasPayload() const;

    SkywireAtEngine _at;
    const __FlashStringHelper *_flash_message;
    char *_message;
    unsigned long _last_read_timestamp;
    uint8_t _send_flags;
};

static_assert(SkywireCommandConcept<SocketSendSkywireCommand>, "SocketSendSkywireCommand doesnt implement the concept");

#endif
