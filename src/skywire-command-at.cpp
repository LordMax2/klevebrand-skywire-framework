#include "skywire-command-at.h"

AtSkywireCommand::AtSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content)) : SkywireCommand(skywire, "AT", debug_mode, on_completed_function) 
{

}

SkywireResponseResult_t AtSkywireCommand::process()
{
    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    unsigned long now = millis();

    if (!sent)
    {
        skywire->print(command + "\r");
        sent = true;
        sent_timestamp = now;
        return SkywireResponseResult_t(false, "");
    }

    serialReadToRxBuffer();

    if (okReceived())
    {
        if (debug_mode)
        {
            Serial.println(rx_buffer);
            Serial.println("STEPPER CLIENT STEP: " + command + " RECEIVED OK");
        }

        if (completed() && on_completed_function != nullptr && !on_completed_called)
        {
            on_completed_function(rx_buffer);
            on_completed_called = true;
        }

        return SkywireResponseResult_t(true, rx_buffer);
    }

    return SkywireResponseResult_t(false, "");
}