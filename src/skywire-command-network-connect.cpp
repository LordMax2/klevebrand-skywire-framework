#include "skywire-command-network-connect.h"

NetworkConnectSkywireCommand::NetworkConnectSkywireCommand(HardwareSerial *skywire, bool debug_mode, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "", debug_mode, on_completed_function)
{
}

SkywireResponseResult_t NetworkConnectSkywireCommand::process()
{
    auto rx_buffer = getRxBuffer();

    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    const unsigned long now = millis();

    setFirstProcessCall();

    if (!isSent())
    {
        if (getFirstProcessCallTimestamp() > 200 && getFirstProcessCallTimestamp() != 0)
        {
            if (debug_mode)
            {
                Serial.println("NETWORK CONNECT Sending command: AT+CEREG?\r");
            }
            skywire->print("AT+CEREG?\r");

            setSent(true);
        }

        return SkywireResponseResult_t(false, "");
    }

    serialReadToRxBuffer();

    rx_buffer = getRxBuffer();

    const bool has_ok = okReceived();
    if (debug_mode && has_ok)
    {
        Serial.println(rx_buffer);
        Serial.println("STEPPER CLIENT RECEIVED CEREG");
    }

    if (isNetworkConnected())
    {
        if (on_completed_function != nullptr && !isOnCompletedCalled())
        {
            on_completed_function(rx_buffer);
            setOnCompletedCalled(true);
        }

        return SkywireResponseResult_t(true, rx_buffer);
    }

    return SkywireResponseResult_t(false, "");
}

bool NetworkConnectSkywireCommand::isNetworkConnected()
{
    auto rx_buffer = getRxBuffer();

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
