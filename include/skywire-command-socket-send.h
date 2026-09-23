#pragma once

#ifndef SKYWIRE_COMMAND_SOCKET_SEND_H
#define SKYWIRE_COMMAND_SOCKET_SEND_H

#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize, size_t SocketReceiveSize>
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
    [[nodiscard]] bool completed() const;
    void reset();

    [[nodiscard]] unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    [[nodiscard]] const __FlashStringHelper *getCommand() const { return _at.getCommand(); }

    [[nodiscard]] char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    [[nodiscard]] bool promptReceived() const;
    [[nodiscard]] bool responseReceived() const;
    [[nodiscard]] bool responseRetrySuggested() const;
    [[nodiscard]] bool shouldRetrySocketRead() const;
    [[nodiscard]] int receivedByteCount() const;
    void writePayloadToModem();
    void readSocketResponse();
    [[nodiscard]] bool hasPayload() const;

    SkywireAtEngine<RxBufferSize> _at;
    const __FlashStringHelper *_flash_message;
    char *_message;
    unsigned long _last_response_request_timestamp;
    bool _should_read_response;
    bool _has_sent_payload;
    bool _has_requested_response;
};

#include "skywire-command-socket-send.ipp"

#endif
