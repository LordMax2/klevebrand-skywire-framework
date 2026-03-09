#include "skywire-command-at.h"

AtSkywireCommand::AtSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function) : SkywireCommand(skywire, "AT", debug_mode, on_completed_function)
{
}

SkywireResponseResult_t AtSkywireCommand::process()
{
    auto rx_buffer = getRxBuffer();

    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    const unsigned long now = millis();

    setFirstProcessCall();

    if (!isSent())
    {
        if (now - getFirstProcessCallTimestamp() > 200 && getFirstProcessCallTimestamp() != 0)
        {
            skywire->print(command);
            skywire->print("\r");

            setSent(true);
        }

        return SkywireResponseResult_t(false, "");
    }

    serialReadToRxBuffer();

    rx_buffer = getRxBuffer();
    const bool has_ok = okReceived();

    if (has_ok)
    {
        if (debug_mode)
        {
            Serial.println(rx_buffer);
            Serial.println("STEPPER CLIENT STEP: " + String(command) + " RECEIVED OK");
        }

        const bool is_complete = completed();
        if (is_complete && on_completed_function != nullptr && !isOnCompletedCalled())
        {
            on_completed_function(rx_buffer);

            setOnCompletedCalled(true);
        }

        return SkywireResponseResult_t(true, rx_buffer);
    }

    if (now - getSentTimestamp() >= 1000)
    {
        if (debug_mode)
        {
            Serial.println("AT command did not receive a response; retrying...");
        }

        skywire->print(command);
        skywire->print("\r");

        setSent(true);
    }

    return SkywireResponseResult_t(false, "");
}