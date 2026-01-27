#ifndef WAIT_FOR_HTTP_RING_H
#define WAIT_FOR_HTTP_RING_H

#include "Arduino.h"
#include "klevebrand-skywire-framework.h"
#include "klevebrand-skywire-framework-step.h"

class HttpRingSkywireStep : public SkywireStep
{
public:
	HttpRingSkywireStep(Skywire &skywire, bool debug_mode, void (*on_completed_function)(String result_content)) : SkywireStep(skywire, "", debug_mode, on_completed_function) {}
	SkywireResponseResult_t process() override
	{
		if (completed())
		{
			return SkywireResponseResult_t(true, rx_buffer);
		}

		sent = true;

		serialReadToRxBuffer();

		if (debug_mode && okReceived())
		{
			Serial.println(rx_buffer);
			Serial.println("STEPPER CLIENT RECEIVED HTTPRING OK");
		}

		return SkywireResponseResult_t(false, "");
	};

	bool okReceived() override
	{
		return rx_buffer.indexOf("HTTPRING") != -1 && rx_buffer.indexOf("\r\n") != -1 && !skywire.available();
	}
};

#endif // WAIT_FOR_HTTP_RING_H