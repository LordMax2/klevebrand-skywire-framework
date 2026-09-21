#pragma once

#ifndef SKYWIRE_AT_ENGINE_H
#define SKYWIRE_AT_ENGINE_H

#include "Arduino.h"
#include "skywire_config.h"
#include "skywire-response-result.h"
#include "concept_skywire_command.h"

typedef void (*OnCompletedFunction)(char *result_content);

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
    unsigned long getSentTimestamp() const;
    const __FlashStringHelper *command() const;

    bool okReceived() const;
    bool waitForSendThenRead();
    void writeCommandToModem();
    void serialReadToRxBuffer();
    void resetRxBuffer();
    static char *getRxBuffer();

    void setFirstProcessCall();
    unsigned long getFirstProcessCallTimestamp() const;
    bool isSent() const;
    void setSent(bool sent);
    void setCompleted(bool completed);
    bool isCompletedFlag() const;
    bool isOnCompletedCalled() const;
    void setOnCompletedCalled(bool on_completed_called);
    void notifyCompletedIfNeeded();

    void printToModem(const __FlashStringHelper *value);
    void printToModem(const char *value);
    void printToModem(char value);
    void printToModem(int value);
    void writeToModem(uint8_t value);
    bool modemAvailable() const;

    static bool debugMode();
    static void rebeginModem();
    static void logStepTimeout(
        const __FlashStringHelper *command,
        unsigned long timeout_milliseconds,
        unsigned long sent_timestamp);

private:
    static HardwareSerial *_skywire;
    static bool _debug_mode;
    static char _rx_buffer[SKYWIRE_RX_BUFFER_SIZE];
    static size_t _rx_buffer_cursor_index;

    const __FlashStringHelper *_command;
    OnCompletedFunction _on_completed_function;
    unsigned long _sent_timestamp;
    unsigned long _first_process_call_timestamp;
    bool _sent;
    bool _on_completed_called;
    bool _first_process_called;
    bool _completed;
};

static_assert(SkywireCommandConcept<SkywireAtEngine>, "SkywireAtEngine doesnt implement the concept");

#endif
