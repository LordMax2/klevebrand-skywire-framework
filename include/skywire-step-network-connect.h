#ifndef SKYWIRE_STEP_NETWORK_CONNECT_H
#define SKYWIRE_STEP_NETWORK_CONNECT_H

#include "Arduino.h"
#include "klevebrand-skywire-framework.h"
#include "skywire-step.h"

class NetworkConnectSkywireStep : public SkywireStep
{
public:
    NetworkConnectSkywireStep(Skywire &skywire, bool debug_mode, void (*on_completed_function)(String &result_content)) : SkywireStep(skywire, "", debug_mode, on_completed_function) {}

    SkywireResponseResult_t process() override
    {
        if (completed())
        {
            return SkywireResponseResult_t(true, rx_buffer);
        }

        if (!sent)
        {
            skywire.print("AT+CEREG?\r");

            sent = true;
            sent_timestamp = millis();

            return SkywireResponseResult_t(false, "");
        }

        serialReadToRxBuffer();

        if (debug_mode && okReceived())
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

private:
    bool isNetworkConnected()
    {
        int cereg_index = rx_buffer.indexOf("+CEREG:");

        if (cereg_index == -1)
            return false;

        String remainder = rx_buffer.substring(cereg_index);
        int first_comma = remainder.indexOf(',');

        if (first_comma == -1)
            return false;

        String status_section = remainder.substring(first_comma + 1);

        if (status_section.length() == 0)
            return false;

        char status_char = status_section[0];

        if (status_char == '1' || status_char == '5')
        {
            return true;
        }

        return false;
    }
};

#endif // SKYWIRE_STEP_NETWORK_CONNECT_H
