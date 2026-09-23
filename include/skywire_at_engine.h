#pragma once

#ifndef SKYWIRE_AT_ENGINE_H
#define SKYWIRE_AT_ENGINE_H

#include "Arduino.h"
#include "skywire-response-result.h"
#include "skywire_on_completed_function.h"
#include "skywire_flash_string.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize>
class SkywireAtEngine
{
public:
    SkywireAtEngine(
        HardwareSerial *skywire,
        const __FlashStringHelper *command,
        bool debug_mode,
        OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    void reset();
    [[nodiscard]] bool completed() const;
    [[nodiscard]] unsigned long getLastSendTimestamp() const;
    [[nodiscard]] const __FlashStringHelper *getCommand() const;

    [[nodiscard]] bool okReceived() const;
    bool waitForSendThenRead();
    void writeCommandToModem();
    void serialReadToRxBuffer();
    void resetRxBuffer();
    static char *getRxBuffer();

    void recordFirstProcessCall();
    [[nodiscard]] unsigned long getFirstProcessCallTimestamp() const;
    [[nodiscard]] bool hasSent() const;
    void setSent(bool has_sent);
    void setCompleted(bool is_completed);
    [[nodiscard]] bool hasMarkedCompleted() const;
    [[nodiscard]] bool hasCalledOnCompleted() const;
    void setHasCalledOnCompleted(bool has_called_on_completed);
    void notifyCompletedIfNeeded();

    void printToModem(const __FlashStringHelper *value);
    void printToModem(const char *value);
    void printToModem(char value);
    void printToModem(int value);
    void writeToModem(uint8_t value);
    [[nodiscard]] bool modemAvailable() const;

    static bool debugMode();
    static void beginModem();

private:
    inline static HardwareSerial *_skywire = nullptr;
    inline static bool _debug_mode = false;
    inline static char _rx_buffer[RxBufferSize] = {0};
    inline static size_t _rx_buffer_cursor_index = 0;

    const __FlashStringHelper *_command;
    OnCompletedFunction _on_completed_function;
    unsigned long _last_send_timestamp;
    unsigned long _first_process_call_timestamp;
    bool _has_sent;
    bool _has_called_on_completed;
    bool _has_recorded_first_process_call;
    bool _has_marked_completed;
};

#include "skywire_at_engine.ipp"

#endif
