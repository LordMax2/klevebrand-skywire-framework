#include "skywire-command-httprcv.h"

#if SKYWIRE_ENABLE_HTTP

#include "skywire_strstr_p.h"

HttpRcvSkywireCommand::HttpRcvSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("HTTPRCV"), debug_mode, on_completed_function),
      _timestamp_milliseconds(0)
{
}

SkywireResponseResult_t HttpRcvSkywireCommand::process()
{
    char *rx_buffer = SkywireAtEngine::getRxBuffer();

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
            if (SkywireAtEngine::debugMode())
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

        if (SkywireAtEngine::debugMode())
        {
            Serial.println(F("HTTPRCV Sending command: AT#HTTPRCV=0,64\r"));
        }
    }

    _at.serialReadToRxBuffer();
    rx_buffer = SkywireAtEngine::getRxBuffer();

    const bool has_ok = okReceived();
    if (SkywireAtEngine::debugMode() && has_ok)
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

bool HttpRcvSkywireCommand::okReceived() const
{
    return skywireContainsP(SkywireAtEngine::getRxBuffer(), PSTR("ERROR"));
}

void HttpRcvSkywireCommand::reset()
{
    _at.reset();
    _timestamp_milliseconds = 0;
}

bool HttpRcvSkywireCommand::completed() const
{
    return _at.hasMarkedCompleted() || (_at.hasSent() && okReceived());
}

#endif
