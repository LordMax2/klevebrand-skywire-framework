#include "skywire-command-httpsnd.h"

HttpSndSkywireCommand::HttpSndSkywireCommand(HardwareSerial *skywire, bool debug_mode, const char path[256], OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "AT#HTTPSND=0,0,", debug_mode, on_completed_function)
{
    strncpy(this->path, path, sizeof(this->path) - 1);
    this->path[sizeof(this->path) - 1] = '\0';
}

bool HttpSndSkywireCommand::arrowsReceived()
{
    auto rx_buffer = getRxBuffer();

    return strstr(rx_buffer, ">>>") != nullptr || millis() - getSentTimestamp() > 200;
}

void HttpSndSkywireCommand::setPayload(char payload[1024])
{
    strncpy(this->payload, payload, sizeof(this->payload) - 1);
    this->payload[sizeof(this->payload) - 1] = '\0';
}

char* HttpSndSkywireCommand::getPayload()
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
                Serial.println("HTTPSND Sending command: " + String(command) + "," + String(payload.length()) + "\r");
            }
            skywire->print(command);
            skywire->print(",");
            skywire->print(payload.length());
            skywire->print("\r");

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
            Serial.print("STEPPER CLIENT RECEIVED HTTPSND OK: ");
            Serial.println(rx_buffer);
            Serial.println("--- END OF RX BUFFER ---");
        }

        on_completed_function(rx_buffer);

        setOnCompletedCalled(true);
    }

    return SkywireResponseResult_t(false, "");
}

bool HttpSndSkywireCommand::completed()
{
    return (isSent() && okReceived() && arrowsReceived() && payload_sent) || getPayload()[0] == '\0';
}