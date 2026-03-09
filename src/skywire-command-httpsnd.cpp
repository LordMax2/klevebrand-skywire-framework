#include "skywire-command-httpsnd.h"

HttpSndSkywireCommand::HttpSndSkywireCommand(HardwareSerial* skywire, bool debug_mode, String path, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "AT#HTTPSND=0,0," + path, debug_mode, on_completed_function)
{
}

bool HttpSndSkywireCommand::arrowsReceived()
{
    return _rx_buffer.indexOf(">>>") != -1 || millis() - _sent_timestamp > 200;
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
    if (completed())
    {
        return SkywireResponseResult_t(true, _rx_buffer);
    }

    const String payload = getPayload();
    const unsigned long now = millis();

    if (_first_process_call)
    {
        _first_process_call = false;
        _first_process_call_timestamp = now;
    }

    if (!_sent && now - _first_process_call_timestamp > 200)
    {
        if (debug_mode)
        {
            Serial.println("HTTPSND Sending command: " + command + "," + String(payload.length()) + "\r");
        }
        skywire->print(command + "," + String(payload.length()) + "\r");

        _sent = true;
        _sent_timestamp = now;

        return SkywireResponseResult_t(false, "");
    }

    if (!_sent)
    {
        return SkywireResponseResult_t(false, "");
    }

    if (!payload_sent)
    {
        serialReadToRxBuffer();
    }

    if (now - _sent_timestamp > 500 && !payload_sent)
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
    if (is_complete && on_completed_function != nullptr && !_on_completed_called)
    {
        if (debug_mode)
        {
            Serial.println("STEPPER CLIENT RECEIVED HTTPSND OK: " + _rx_buffer);
        }

        on_completed_function(_rx_buffer);

        _on_completed_called = true;
    }

    return SkywireResponseResult_t(false, "");
}

bool HttpSndSkywireCommand::completed()
{
    return (_sent && okReceived() && arrowsReceived() && payload_sent) || getPayload() == "";
}