#include "skywire-command.h"

SkywireCommand::SkywireCommand(HardwareSerial* skywire, String command, bool debug_mode, void (*on_completed_function)(String& result_content))
    : skywire(skywire), command(command), debug_mode(debug_mode), on_completed_function(on_completed_function)
{
}

void SkywireCommand::serialReadToRxBuffer()
{
    while (skywire->available())
    {
        char c = skywire->read();
        rx_buffer += c;
    }
}

void SkywireCommand::resetRxBuffer()
{
    rx_buffer = "";
}

bool SkywireCommand::okReceived() 
{
    return true;
}

void SkywireCommand::resetState()
{
    sent = false;
    sent_timestamp = 0;
    on_completed_called = false;
    first_process_call = true;
    first_process_call_timestamp = 0;

    resetRxBuffer();
}

bool SkywireCommand::completed()
{
    return sent && okReceived();
}

SkywireResponseResult_t SkywireCommand::process()
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
        skywire->print(command + "\r");

        sent = true;
        sent_timestamp = now;

        return SkywireResponseResult_t(false, "");
    }

    if (!sent)
    {
        return SkywireResponseResult_t(false, "");
    }

    serialReadToRxBuffer();

    const bool has_ok = okReceived();
    if (has_ok)
    {
        if (debug_mode)
        {
            Serial.println(rx_buffer);
            Serial.println("STEPPER CLIENT STEP: " + command + "RECEIVED OK");
        }

        const bool is_complete = completed();
        if (is_complete && on_completed_function != nullptr && !on_completed_called)
        {
            on_completed_function(rx_buffer);

            on_completed_called = true;
        }

        return SkywireResponseResult_t(true, rx_buffer);
    }

    return SkywireResponseResult_t(false, "");
}

