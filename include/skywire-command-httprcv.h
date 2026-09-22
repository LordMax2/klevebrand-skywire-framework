#pragma once

#ifndef SKYWIRE_COMMAND_HTTPRCV_H
#define SKYWIRE_COMMAND_HTTPRCV_H



#include "skywire_at_engine.h"
#include "concept_skywire_command.h"

template<size_t RxBufferSize>
class HttpRcvSkywireCommand
{
public:
    HttpRcvSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process();
    void reset();
    bool completed() const;

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *getCommand() const { return _at.getCommand(); }

    char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    bool okReceived() const;

    SkywireAtEngine<RxBufferSize> _at;
    unsigned long _timestamp_milliseconds;
};




template<size_t RxBufferSize>
HttpRcvSkywireCommand<RxBufferSize>::HttpRcvSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("HTTPRCV"), debug_mode, on_completed_function),
      _timestamp_milliseconds(0)
{
    static_assert(SkywireCommandConcept<HttpRcvSkywireCommand<RxBufferSize>>, "HttpRcvSkywireCommand doesnt implement the concept");
}

template<size_t RxBufferSize>
SkywireResponseResult_t HttpRcvSkywireCommand<RxBufferSize>::process()
{
    char *rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    const unsigned long now = millis();

    _at.recordFirstProcessCall();

    if (!_at.hasSent())
    {
        if (now - _at.getFirstProcessCallTimestamp() > 200 && _at.getFirstProcessCallTimestamp() != 0)
        {
            _at.resetRxBuffer();
            if (SkywireAtEngine<RxBufferSize>::debugMode())
            {
                Serial.println(F("HTTPRCV Sending command: AT#HTTPRCV=0,64\r"));
            }

            _at.printToModem(F("AT#HTTPRCV=0,64\r"));
            _at.setSent(true);
        }

        return {false, rx_buffer};
    }

    if (now - _timestamp_milliseconds > 200 && !_at.modemAvailable())
    {
        _timestamp_milliseconds = now;
        _at.resetRxBuffer();
        _at.printToModem(F("AT#HTTPRCV=0,64\r"));

        if (SkywireAtEngine<RxBufferSize>::debugMode())
        {
            Serial.println(F("HTTPRCV Sending command: AT#HTTPRCV=0,64\r"));
        }
    }

    _at.serialReadToRxBuffer();
    rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    const bool has_ok = okReceived();
    if (SkywireAtEngine<RxBufferSize>::debugMode() && has_ok)
    {
        Serial.println(F("STEPPER CLIENT RECEIVED HTTPRCV CONTENT"));
        Serial.println(rx_buffer);
        Serial.println(F("--- END OF RX BUFFER ---"));
    }

    if (completed())
    {
        _at.notifyCompletedIfNeeded();
        _at.setCompleted(true);
    }

    return {false, rx_buffer};
}

template<size_t RxBufferSize>
bool HttpRcvSkywireCommand<RxBufferSize>::okReceived() const
{
    return skywireContainsFlashString(SkywireAtEngine<RxBufferSize>::getRxBuffer(), PSTR("ERROR"));
}

template<size_t RxBufferSize>
void HttpRcvSkywireCommand<RxBufferSize>::reset()
{
    _at.reset();
    _timestamp_milliseconds = 0;
}

template<size_t RxBufferSize>
bool HttpRcvSkywireCommand<RxBufferSize>::completed() const
{
    return _at.hasMarkedCompleted() || (_at.hasSent() && okReceived());
}

#endif
