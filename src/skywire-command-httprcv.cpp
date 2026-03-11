#include "skywire-command-httprcv.h"

HttpRcvSkywireCommand::HttpRcvSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "HTTPRCV", debug_mode, on_completed_function)
{
}

SkywireResponseResult_t HttpRcvSkywireCommand::process()
{
    auto rx_buffer = getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    const unsigned long now = millis();

    setFirstProcessCall();

    if (!isSent())
    {
        if (getFirstProcessCallTimestamp() > 200 && getFirstProcessCallTimestamp() != 0)
        {
            if (debug_mode)
            {
                Serial.println("HTTPRCV Sending command: AT#HTTPRCV=0,64\r");
            }
            skywire->print("AT#HTTPRCV=0,64\r");

            setSent(true);
        }

        return SkywireResponseResult_t(false, "");
    }

    if (now - timestamp_milliseconds > 200 && !skywire->available())
    {
        timestamp_milliseconds = now;

        skywire->print("AT#HTTPRCV=0,64\r");
    }

    serialReadToRxBuffer();

    const bool has_ok = okReceived();
    if (debug_mode && has_ok)
    {
        Serial.println("STEPPER CLIENT RECEIVED HTTPRCV CONTENT");
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

bool HttpRcvSkywireCommand::okReceived()
{
    const auto rx_buffer = getRxBuffer();

    return strstr(rx_buffer, "ERROR") != nullptr;
}
