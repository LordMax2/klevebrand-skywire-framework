#include "skywire-command-httpring.h"

#if SKYWIRE_ENABLE_HTTP

#include "skywire_strstr_p.h"

HttpRingSkywireCommand::HttpRingSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("HTTPRING"), debug_mode, on_completed_function)
{
}

SkywireResponseResult_t HttpRingSkywireCommand::process()
{
    char *const rx_buffer = SkywireAtEngine::getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    _at.recordFirstProcessCall();

    if (!_at.hasSent())
    {
        _at.resetRxBuffer();
        _at.setSent(true);
    }

    _at.serialReadToRxBuffer();

    const bool has_ok = okReceived();
    if (SkywireAtEngine::debugMode() && has_ok)
    {
        Serial.println(F("STEPPER CLIENT RECEIVED HTTPRING OK: "));
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

bool HttpRingSkywireCommand::okReceived() const
{
    char *const ring = skywireStrstrP(SkywireAtEngine::getRxBuffer(), PSTR("HTTPRING"));
    if (ring == nullptr)
    {
        return false;
    }

    return strchr(ring, '\r') != nullptr;
}

bool HttpRingSkywireCommand::completed() const
{
    return _at.hasMarkedCompleted() ||
           (_at.hasSent() && okReceived() && millis() - _at.getSentTimestamp() > 500);
}

#endif
