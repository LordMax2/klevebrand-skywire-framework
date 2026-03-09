#include "skywire-command-network-connect.h"

NetworkConnectSkywireCommand::NetworkConnectSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "", debug_mode, on_completed_function)
{
}

SkywireResponseResult_t NetworkConnectSkywireCommand::process()
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
        skywire->print("AT+CEREG?\r");

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
    if (debug_mode && has_ok)
    {
        Serial.println(_rx_buffer);
        Serial.println("STEPPER CLIENT RECEIVED CEREG");
    }

    if (isNetworkConnected())
    {
        if (on_completed_function != nullptr && !_on_completed_called)
        {
            on_completed_function(_rx_buffer);
            _on_completed_called = true;
        }

        return SkywireResponseResult_t(true, _rx_buffer);
    }

    return SkywireResponseResult_t(false, "");
}

bool NetworkConnectSkywireCommand::isNetworkConnected()
{
    int cereg_index = _rx_buffer.indexOf("+CEREG:");

    if (cereg_index == -1)
    {
        return false;
    }

    String remainder = _rx_buffer.substring(cereg_index);
    int first_comma = remainder.indexOf(',');

    if (first_comma == -1)
    {
        return false;
    }

    String status_section = remainder.substring(first_comma + 1);

    if (status_section.length() == 0)
    {
        return false;
    }

    char status_char = status_section[0];

    if (status_char == '1' || status_char == '5')
    {
        return true;
    }

    return false;
}
