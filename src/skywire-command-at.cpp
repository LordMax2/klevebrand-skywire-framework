#include "skywire-command-at.h"

AtSkywireCommand::AtSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content)) : SkywireCommand(skywire, "AT", debug_mode, on_completed_function) 
{

}

SkywireResponseResult_t AtSkywireCommand::process()
{
    if (completed())
    {
        return SkywireResponseResult_t(true, _rx_buffer);
    }

    const unsigned long now = millis();

    if (_first_process_call)
    {
        _first_process_call = false;
        _first_process_call_timestamp = now;
    }

    if (!_sent && now - _first_process_call_timestamp > 200)
    {
        skywire->print(command + "\r");
        _sent = true;
        _sent_timestamp = now;
        return SkywireResponseResult_t(false, "");
    }

    if (!_sent)
    {
        return SkywireResponseResult_t(false, "");
    }

    serialReadToRxBuffer();

    const bool has_ok = okReceived();
    if (has_ok)
    {
        if (debug_mode)
        {
            Serial.println(_rx_buffer);
            Serial.println("STEPPER CLIENT STEP: " + command + " RECEIVED OK");
        }

        const bool is_complete = completed();
        if (is_complete && on_completed_function != nullptr && !_on_completed_called)
        {
            on_completed_function(_rx_buffer);
            _on_completed_called = true;
        }

        return SkywireResponseResult_t(true, _rx_buffer);
    }

    if (now - _sent_timestamp >= 1000)
    {
        if (debug_mode)
        {
            Serial.println("AT command did not receive a response; retrying...");
        }
        skywire->print(command + "\r");
        _sent_timestamp = now;
    }

    return SkywireResponseResult_t(false, "");
}