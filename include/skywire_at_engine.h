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
    bool completed() const;
    unsigned long getLastSendTimestamp() const;
    const __FlashStringHelper *getCommand() const;

    bool okReceived() const;
    bool waitForSendThenRead();
    void writeCommandToModem();
    void serialReadToRxBuffer();
    void resetRxBuffer();
    static char *getRxBuffer();

    void recordFirstProcessCall();
    unsigned long getFirstProcessCallTimestamp() const;
    bool hasSent() const;
    void setSent(bool has_sent);
    void setCompleted(bool is_completed);
    bool hasMarkedCompleted() const;
    bool hasCalledOnCompleted() const;
    void setHasCalledOnCompleted(bool has_called_on_completed);
    void notifyCompletedIfNeeded();

    void printToModem(const __FlashStringHelper *value);
    void printToModem(const char *value);
    void printToModem(char value);
    void printToModem(int value);
    void writeToModem(uint8_t value);
    bool modemAvailable() const;

    static bool debugMode();
    static void beginModem();

private:
    static HardwareSerial *_skywire;
    static bool _debug_mode;
    static char _rx_buffer[RxBufferSize];
    static size_t _rx_buffer_cursor_index;

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
