#ifndef STEP_AT
#define STEP_AT

#include "skywire-step.h"

class AtSkywireStep : public SkywireStep {
public:
    AtSkywireStep(HardwareSerial &skywire, bool debug_mode, void (*on_completed_function)(String &result_content)) : SkywireStep(skywire, "AT", debug_mode, on_completed_function) {}

    // send AT commands repeatedly once per second until a response arrives
    virtual SkywireResponseResult_t process() override
    {
        if (completed())
        {
            return SkywireResponseResult_t(true, rx_buffer);
        }

        unsigned long now = millis();

        if (!sent)
        {
            skywire.print(command + "\r");
            sent = true;
            sent_timestamp = now;
            return SkywireResponseResult_t(false, "");
        }

        serialReadToRxBuffer();

        if (okReceived())
        {
            if (debug_mode)
            {
                Serial.println(rx_buffer);
                Serial.println("STEPPER CLIENT STEP: " + command + " RECEIVED OK");
            }

            if (completed() && on_completed_function != nullptr && !on_completed_called)
            {
                on_completed_function(rx_buffer);
                on_completed_called = true;
            }

            return SkywireResponseResult_t(true, rx_buffer);
        }

        if (now - sent_timestamp >= 1000)
        {
            if (debug_mode)
            {
                Serial.println("AT command did not receive a response; retrying...");
            }
            skywire.print(command + "\r");
            sent_timestamp = now;
        }

        return SkywireResponseResult_t(false, "");
    }
};

#endif // STEP_AT