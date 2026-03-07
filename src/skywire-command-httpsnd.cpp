#include "skywire-command-httpsnd.h"

HttpSndSkywireCommand::HttpSndSkywireCommand(HardwareSerial* skywire, bool debug_mode, String path, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "AT#HTTPSND=0,0," + path, debug_mode, on_completed_function)
{
}

SkywireResponseResult_t HttpSndSkywireCommand::process(String payload)
{
    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    if (!sent)
    {
        skywire->print(command + "," + String(payload.length()) + "\r\x1A");

        sent = true;

        sent_timestamp = millis();

        return SkywireResponseResult_t(false, "");
    }

    serialReadToRxBuffer();

    if(debug_mode && arrowsReceived())
    {
        Serial.println(rx_buffer);
        Serial.println("STEPPER CLIENT RECEIVED >>>: " + rx_buffer);
    }

    if(arrowsReceived() && !payload_sent)
    {
        skywire->print(payload + "\r");

        payload_sent = true;
    }

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

bool HttpSndSkywireCommand::completed()
{
    return sent && okReceived() && arrowsReceived() && payload_sent;
}