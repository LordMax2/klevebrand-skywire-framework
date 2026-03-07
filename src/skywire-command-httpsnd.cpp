#include "skywire-command-httpsnd.h"

HttpSndSkywireCommand::HttpSndSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "AT#HTTPSND=1,0", debug_mode, on_completed_function)
{
}

SkywireResponseResult_t HttpSndSkywireCommand::process()
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

    if (debug_mode && okReceived())
    {
        Serial.println(rx_buffer);
        Serial.println("STEPPER CLIENT RECEIVED HTTPSND OK: " + rx_buffer);
    }

    if (completed() && on_completed_function != nullptr && !on_completed_called)
    {
        on_completed_function(rx_buffer);

        on_completed_called = true;
    }

    return SkywireResponseResult_t(false, "");
}