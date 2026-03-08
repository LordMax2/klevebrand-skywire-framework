#include "skywire-command-httpsnd.h"

HttpSndSkywireCommand::HttpSndSkywireCommand(HardwareSerial* skywire, bool debug_mode, String path, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "AT#HTTPSND=0,0," + path, debug_mode, on_completed_function)
{
}

bool HttpSndSkywireCommand::arrowsReceived()
{
    return rx_buffer.indexOf(">>>") != -1 || millis() - sent_timestamp > 200;
}

void HttpSndSkywireCommand::setPayload(String payload)
{
    this->payload = payload;
}

String HttpSndSkywireCommand::getPayload()
{
    return payload;
}

void HttpSndSkywireCommand::resetState()
{
    SkywireCommand::resetState();
    payload_sent = false;
}

SkywireResponseResult_t HttpSndSkywireCommand::process()
{
    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    String payload = getPayload();

    if(first_process_call)
    {
        first_process_call = false;
        first_process_call_timestamp = millis();
    }

    if (!sent && millis() - first_process_call_timestamp > 200)
    {
        if(debug_mode)
        {
            Serial.println("HTTPSND Sending command: " + command + "," + String(payload.length()) + "\r");
        }
        skywire->print(command + "," + String(payload.length()) + "\r");

        sent = true;

        sent_timestamp = millis();

        return SkywireResponseResult_t(false, "");
    }

    if (!sent)
    {
        return SkywireResponseResult_t(false, "");
    }

    if (!payload_sent)
    {
        serialReadToRxBuffer();
    }

    if(millis() - sent_timestamp > 500 && !payload_sent)
    {
        if(debug_mode)
        {
            Serial.println("HTTPSND Sending payload: " + payload);
        }
        skywire->print(payload + "\x1A");

        payload_sent = true;

        if(debug_mode)
        {
            Serial.println("HTTPSND is completed: " + String(completed()));
        }
    }

    if (completed() && on_completed_function != nullptr && !on_completed_called)
    {
        if(debug_mode)
        {
            Serial.println("STEPPER CLIENT RECEIVED HTTPSND OK: " + rx_buffer);
        }

        on_completed_function(rx_buffer);

        on_completed_called = true;
    }

    return SkywireResponseResult_t(false, "");
}

bool HttpSndSkywireCommand::completed()
{
    return (sent && okReceived() && arrowsReceived() && payload_sent) || getPayload() == "";
}