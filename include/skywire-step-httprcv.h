#ifndef WAIT_FOR_HTTP_RCV_H
#define WAIT_FOR_HTTP_RCV_H

#include "Arduino.h"
#include "klevebrand-skywire-framework.h"
#include "skywire-step.h"

class HttpRcvSkywireStep : public SkywireStep
{
public:
    HttpRcvSkywireStep(HardwareSerial &skywire, bool debug_mode, void (*on_completed_function)(String& result_content)) : SkywireStep(skywire, "", debug_mode, on_completed_function) {}
    SkywireResponseResult_t process() override
    {
		if(completed())
		{
			return SkywireResponseResult_t(true, rx_buffer);
		}

        if(!sent)
        {
            skywire.print("AT#HTTPRCV=0,64\r");

            sent = true;

            timestamp_milliseconds = millis();

            return SkywireResponseResult_t(false, "");
        }

        if (sent && millis() - timestamp_milliseconds > 200 && !skywire.available())
        {
            timestamp_milliseconds = millis();

            skywire.print("AT#HTTPRCV=0,64\r");
        }

        serialReadToRxBuffer();

        if (debug_mode && okReceived())
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

    bool okReceived() override
    {
        return rx_buffer.indexOf("ERROR") != -1;
    }

private:
    unsigned long timestamp_milliseconds = 0;
};

#endif // WAIT_FOR_HTTP_RCV_H