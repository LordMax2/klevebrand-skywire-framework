#include "skywire-command-httpring.h"

HttpRingSkywireCommand::HttpRingSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "HTTPRING", debug_mode, on_completed_function)
{
}

SkywireResponseResult_t HttpRingSkywireCommand::process()
{
    auto rx_buffer = getRxBuffer();

    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    setFirstProcessCall();

    if (!isSent())
        setSent(true);

    serialReadToRxBuffer();

    const bool has_ok = okReceived();
    if (debug_mode && has_ok)
    {
        Serial.println("STEPPER CLIENT RECEIVED HTTPRING OK: ");
        Serial.println(rx_buffer);
        Serial.println("--- END OF RX BUFFER ---");
    }
    
    const bool is_complete = completed();
    if (is_complete)
    {
        setCompleted(true);

        if (on_completed_function != nullptr && !isOnCompletedCalled())
        {
            on_completed_function(rx_buffer);
            setOnCompletedCalled(true);
        }
    }

    return SkywireResponseResult_t(false, "");
}

bool HttpRingSkywireCommand::okReceived()
{
    auto rx_buffer = getRxBuffer();

    return strstr(rx_buffer, "HTTPRING") != nullptr && strstr(rx_buffer, "\r\n") != nullptr;
}

bool HttpRingSkywireCommand::completed()
{
    return isSent() && okReceived() && (millis() - getSentTimestamp()) > 500;
}
