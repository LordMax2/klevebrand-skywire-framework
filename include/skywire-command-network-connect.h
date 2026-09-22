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

    bool completed() const { return _at.completed(); }

    unsigned long getLastSendTimestamp() const { return _at.getLastSendTimestamp(); }

    const __FlashStringHelper *getCommand() const { return _at.getCommand(); }

    char *getRxBuffer() const { return SkywireAtEngine<RxBufferSize>::getRxBuffer(); }

private:
    static bool isNetworkConnected();

    SkywireAtEngine<RxBufferSize> _at;
    unsigned long _last_poll_timestamp;
};



template<size_t RxBufferSize>
NetworkConnectSkywireCommand<RxBufferSize>::NetworkConnectSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT+CEREG?"), debug_mode, on_completed_function),
      _last_poll_timestamp(0)
{
    static_assert(SkywireCommandConcept<NetworkConnectSkywireCommand<RxBufferSize>>, "NetworkConnectSkywireCommand doesnt implement the concept");
}

template<size_t RxBufferSize>
SkywireResponseResult_t NetworkConnectSkywireCommand<RxBufferSize>::process()
{
    char *rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();
    const unsigned long now = millis();

    if (_at.completed())
    {
        return {true, rx_buffer};
    }

    _at.recordFirstProcessCall();

    if (!_at.hasSent())
    {
        if (now - _at.getFirstProcessCallTimestamp() > 200 &&
            _at.getFirstProcessCallTimestamp() != 0 &&
            (_last_poll_timestamp == 0 || now - _last_poll_timestamp >= 1000))
        {
            if (SkywireAtEngine<RxBufferSize>::debugMode())
            {
                Serial.println(F("NETWORK CONNECT Sending command: AT+CEREG?\r"));
            }

            _at.resetRxBuffer();
            _at.writeCommandToModem();
            _at.setSent(true);
            _last_poll_timestamp = now;
        }

        return {false, rx_buffer};
    }

    _at.serialReadToRxBuffer();
    rx_buffer = SkywireAtEngine<RxBufferSize>::getRxBuffer();

    const bool has_ok = _at.okReceived();
    if (SkywireAtEngine<RxBufferSize>::debugMode() && has_ok)
    {
        Serial.println(F("STEPPER CLIENT RECEIVED CEREG"));
        Serial.println(rx_buffer);
        Serial.println(F("--- END OF RX BUFFER ---"));
    }

    if (isNetworkConnected())
    {
        _at.setCompleted(true);
        _at.notifyCompletedIfNeeded();

        return {true, rx_buffer};
    }

    if (has_ok)
    {
        _at.resetRxBuffer();
        _at.setSent(false);
    }

    return {false, rx_buffer};
}

template<size_t RxBufferSize>
void NetworkConnectSkywireCommand<RxBufferSize>::reset()
{
    _at.reset();
    _last_poll_timestamp = 0;
}

template<size_t RxBufferSize>
bool NetworkConnectSkywireCommand<RxBufferSize>::isNetworkConnected()
{
    char *const rx_ptr = SkywireAtEngine<RxBufferSize>::getRxBuffer();
    char *const cereg_pos = skywireFindFlashString(rx_ptr, PSTR("+CEREG:"));

    if (cereg_pos == nullptr)
    {
        return false;
    }

    const char *first_comma = strchr(cereg_pos, ',');
    if (first_comma == nullptr)
    {
        return false;
    }

    const char status_char = *(first_comma + 1);

    return status_char == '1' || status_char == '5';
}

#endif
