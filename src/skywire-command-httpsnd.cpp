#include "skywire-command-httpsnd.h"

HttpSndSkywireCommand::HttpSndSkywireCommand(HardwareSerial *skywire, bool debug_mode, String path, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "AT#HTTPSND=0,0," + path, debug_mode, on_completed_function)
{
}

bool HttpSndSkywireCommand::arrowsReceived()
{
    auto rx_buffer = getRxBuffer();

    return rx_buffer.indexOf(">>>") != -1 || millis() - getSentTimestamp() > 200;
}

void HttpSndSkywireCommand::setPayload(String payload)
{
    this->payload = payload;
}

String HttpSndSkywireCommand::getPayload()
{
    return payload;
}

void HttpSndSkywireCommand::reset()
{
    SkywireCommand::reset();
    payload_sent = false;
}

SkywireResponseResult_t HttpSndSkywireCommand::process()
{
    auto rx_buffer = getRxBuffer();

    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    const String payload = getPayload();
    const unsigned long now = millis();

    setFirstProcessCall();

    if (!isSent())
    {
        if (getFirstProcessCallTimestamp() > 200 && getFirstProcessCallTimestamp() != 0)
        {

            if (debug_mode)
            {
                Serial.println("HTTPSND Sending command: " + command + "," + String(payload.length()) + "\r");
            }
            skywire->print(command + "," + String(payload.length()) + "\r");

            setSent(true);
        }

        return SkywireResponseResult_t(false, "");
    }

    if (!payload_sent)
    {
        serialReadToRxBuffer();
    }

    if (now - getSentTimestamp() > 500 && !payload_sent)
    {
        if (debug_mode)
        {
            Serial.println("HTTPSND Sending payload: " + payload);
        }
        skywire->print(payload + "\x1A");

        payload_sent = true;

        if (debug_mode)
        {
            Serial.println("HTTPSND is completed: " + String(completed()));
        }
    }

    const bool is_complete = completed();
    if (is_complete && on_completed_function != nullptr && !isOnCompletedCalled())
    {
        if (debug_mode)
        {
            Serial.println("STEPPER CLIENT RECEIVED HTTPSND OK: " + rx_buffer);
        }

        on_completed_function(rx_buffer);

        setOnCompletedCalled(true);
    }

    return SkywireResponseResult_t(false, "");
}

bool HttpSndSkywireCommand::completed()
{
    return (isSent() && okReceived() && arrowsReceived() && payload_sent) || getPayload() == "";
}