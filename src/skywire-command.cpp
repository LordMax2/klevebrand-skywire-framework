#include "skywire-command.h"

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
    return rx_buffer.indexOf("OK") != -1 && rx_buffer.indexOf("\r\n") != -1;
}

void SkywireCommand::resetState()
{
    sent = false;
    sent_timestamp = 0;
    on_completed_called = false;

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

    if (!sent)
    {
        skywire->print(command + "\r");

        sent = true;
        sent_timestamp = millis();

        return SkywireResponseResult_t(false, "");
    }


    serialReadToRxBuffer();

    if (okReceived())
    {
        if (debug_mode)
        {
            Serial.println(rx_buffer);
            Serial.println("STEPPER CLIENT STEP: " + command + "RECEIVED OK");
        }

        if(completed() && on_completed_function != nullptr && !on_completed_called)
        {
            on_completed_function(rx_buffer);

            on_completed_called = true;
        }

        return SkywireResponseResult_t(true, rx_buffer);
    }

    return SkywireResponseResult_t(false, "");
}

