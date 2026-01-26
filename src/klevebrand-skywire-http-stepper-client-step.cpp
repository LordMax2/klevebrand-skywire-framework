#include "klevebrand-skywire-http-stepper-client-step.h"

void SkywireHttpStepperClientStep::serialReadToRxBuffer()
{
    while (skywire.available())
    {
        char c = skywire.read();
        rx_buffer += c;
    }
}

void SkywireHttpStepperClientStep::resetRxBuffer()
{
    rx_buffer = "";
}

void SkywireHttpStepperClientStep::resetState()
{
    sent = false;
    sent_timestamp = 0;
    ok_recieved = false;
    on_completed_called = false;

    resetRxBuffer();
}

bool SkywireHttpStepperClientStep::completed()
{
    return sent && ok_recieved;
}

SkywireResponseResult_t SkywireHttpStepperClientStep::process()
{
    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    if (!sent)
    {
        skywire.print(command + "\r");

        sent = true;
        sent_timestamp = millis();

        return SkywireResponseResult_t(false, "");
    }


    serialReadToRxBuffer();

    ok_recieved = rx_buffer.indexOf("OK") != -1 && rx_buffer.indexOf("\r\n") != -1 && !skywire.available();

    if (ok_recieved)
    {
        if (debug_mode)
        {
            Serial.println(rx_buffer);
            Serial.println("STEPPER CLIENT STEP: " + command + "RECEIVED OK");
        }

        if(completed() && on_completed_function != nullptr && !on_completed_called)
        {
            on_completed_function(rx_buffer);

            on_completed_called = true;
        }

        return SkywireResponseResult_t(true, rx_buffer);
    }

    return SkywireResponseResult_t(false, "");
}