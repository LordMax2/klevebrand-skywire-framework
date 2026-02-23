#ifndef SKYWIRE_HTTP_STEPPER_CLIENT_STEP_H
#define SKYWIRE_HTTP_STEPPER_CLIENT_STEP_H

#include "Arduino.h"
#include "skywire-response-result.h"

class SkywireStep
{
public:
	SkywireStep(HardwareSerial &skywire, String command, bool debug_mode, void (*on_completed_function)(String& result_content)) : skywire(skywire), command(command), debug_mode(debug_mode), on_completed_function(on_completed_function) {}

	virtual bool completed();
	virtual SkywireResponseResult_t process();
	virtual bool okReceived();

	HardwareSerial &skywire;
	String command;
	bool debug_mode;

	void (*on_completed_function)(String& result_content);

	bool sent = false;
	unsigned long sent_timestamp = 0;
	bool on_completed_called = false;

	String rx_buffer = "";

	void serialReadToRxBuffer();
	void resetRxBuffer();
	void resetState();
};

#endif // SKYWIRE_HTTP_STEPPER_CLIENT_STEP_H