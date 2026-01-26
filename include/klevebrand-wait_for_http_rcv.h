#ifndef WAIT_FOR_HTTP_RCV_H
#define WAIT_FOR_HTTP_RCV_H

#include "Arduino.h"
#include "klevebrand-skywire-framework.h"
#include "klevebrand-skywire-http-stepper-client-step.h"

class HttpRcv : public SkywireHttpStepperClientStep
{
public:
    HttpRcv(Skywire &skywire, bool debug_mode, void (*on_oompleted_function)(String result_content)) : SkywireHttpStepperClientStep(skywire, "AT#HTTPRCV=0", debug_mode, on_oompleted_function) {}
    SkywireResponseResult_t process() override
    {
		if(completed())
		{
			return SkywireResponseResult_t(true, rx_buffer);
		}

        if(!sent)
        {
            skywire.print("AT#HTTPRCV=0\r");

            sent = true;

            timestamp_milliseconds = millis();

            return SkywireResponseResult_t(false, "");
        }

        if (sent && millis() - timestamp_milliseconds > 200)
        {
            timestamp_milliseconds = millis();

            skywire.print("AT#HTTPRCV=0\r");
        }

        serialReadToRxBuffer();

        ok_recieved = rx_buffer.indexOf("\r\n") != -1 && !skywire.available() && rx_buffer.indexOf("OK") != -1;

        if (debug_mode && ok_recieved)
        {
            Serial.println(rx_buffer);
            Serial.println("STEPPER CLIENT RECEIVED HTTPRCV CONTENT");
        }

        if(completed() && on_completed_function != nullptr && !on_completed_called)
        {
            on_completed_function(rx_buffer);

            on_completed_called = true;
        }

        return SkywireResponseResult_t(false, "");
    };

private:
    unsigned long timestamp_milliseconds = 0;
};

#endif // WAIT_FOR_HTTP_RCV_H