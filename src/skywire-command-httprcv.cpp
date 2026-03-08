#include "skywire-command-httprcv.h"

HttpRcvSkywireCommand::HttpRcvSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String& result_content))
    : SkywireCommand(skywire, "", debug_mode, on_completed_function)
{
}

SkywireResponseResult_t HttpRcvSkywireCommand::process()
{
    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    const unsigned long now = millis();

    if (first_process_call)
    {
        first_process_call = false;
        first_process_call_timestamp = now;
    }

    if (!sent && now - first_process_call_timestamp > 200)
    {
        skywire->print("AT#HTTPRCV=0,64\r");

        sent = true;
        timestamp_milliseconds = now;

        return SkywireResponseResult_t(false, "");
    }

    if (!sent)
    {
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
        Serial.println(rx_buffer);
        Serial.println("STEPPER CLIENT RECEIVED HTTPRCV CONTENT");
    }

    const bool is_complete = completed();
    if (is_complete && on_completed_function != nullptr && !on_completed_called)
    {
        on_completed_function(rx_buffer);

        on_completed_called = true;
    }

    return SkywireResponseResult_t(false, "");
}

bool HttpRcvSkywireCommand::okReceived()
{
    return rx_buffer.indexOf("ERROR") != -1;
}
