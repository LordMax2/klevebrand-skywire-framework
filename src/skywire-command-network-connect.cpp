#include "skywire-command-network-connect.h"

NetworkConnectSkywireCommand::NetworkConnectSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function)
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

bool NetworkConnectSkywireCommand::isNetworkConnected() {
    auto rx_ptr = getRxBuffer(); 

    char* cereg_pos = strstr(rx_ptr, "+CEREG:");
    if (!cereg_pos) {
        return false;
    }

    char* first_comma = strchr(cereg_pos, ',');
    if (!first_comma) {
        return false;
    }

    char status_char = *(first_comma + 1);

    if (status_char == '1' || status_char == '5') {
        return true;
    }

    return false;
}