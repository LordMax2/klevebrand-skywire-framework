#ifndef WAIT_FOR_HTTP_RING_H
#define WAIT_FOR_HTTP_RING_H

#include "Arduino.h"
#include "klevebrand-skywire-framework.h"
#include "klevebrand-skywire-http-stepper-client-step.h"

class WaitForHttpRing : public SkywireHttpStepperClientStep
{
public:
	WaitForHttpRing(Skywire& skywire, bool debug_mode, void (*on_completed_function)(String result_content)) : SkywireHttpStepperClientStep(skywire, "", debug_mode, on_completed_function) {}
	SkywireResponseResult_t process() override
	{
		if(completed())
		{
			return SkywireResponseResult_t(true, rx_buffer);
		}

		sent = true;

		serialReadToRxBuffer();

		ok_recieved = rx_buffer.indexOf("HTTPRING") != -1 && rx_buffer.indexOf("\r\n") != -1 && !skywire.available();

		if (debug_mode && ok_recieved)
		{
			Serial.println(rx_buffer);
			Serial.println("STEPPER CLIENT RECEIVED HTTPRING OK");
		}

		return SkywireResponseResult_t(false, "");
	};
};

#endif // WAIT_FOR_HTTP_RING_H