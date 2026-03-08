#include "skywire-command-network-connect.h"

NetworkConnectSkywireCommand::NetworkConnectSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "", debug_mode, on_completed_function)
{
}

SkywireResponseResult_t NetworkConnectSkywireCommand::process()
{
    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    const unsigned long now = millis();

    if (first_process_call)
    {
        first_process_call = false;
        first_process_call_timestamp = now;
    }

    if (!sent && now - first_process_call_timestamp > 200)
    {
        skywire->print("AT+CEREG?\r");

        sent = true;
        sent_timestamp = now;

        return SkywireResponseResult_t(false, "");
    }

    if (!sent)
    {
        return SkywireResponseResult_t(false, "");
    }

    serialReadToRxBuffer();

    const bool has_ok = okReceived();
    if (debug_mode && has_ok)
    {
        Serial.println(rx_buffer);
        Serial.println("STEPPER CLIENT RECEIVED CEREG");
    }

    if (isNetworkConnected())
    {
        if (on_completed_function != nullptr && !on_completed_called)
        {
            on_completed_function(rx_buffer);
            on_completed_called = true;
        }

        return SkywireResponseResult_t(true, rx_buffer);
    }

    return SkywireResponseResult_t(false, "");
}

bool NetworkConnectSkywireCommand::isNetworkConnected()
{
    int cereg_index = rx_buffer.indexOf("+CEREG:");

    if (cereg_index == -1)
    {
        return false;
    }

    String remainder = rx_buffer.substring(cereg_index);
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
