#ifndef SKYWIRE_STEP_H
#define SKYWIRE_STEP_H

#include "Arduino.h"
#include "skywire-response-result.h"

class SkywireCommand
{
public:
	SkywireCommand(HardwareSerial* skywire, String command, bool debug_mode, void (*on_completed_function)(String& result_content));

	virtual bool completed();
	virtual SkywireResponseResult_t process();
	virtual bool okReceived();

	HardwareSerial* skywire;
	String command;
	bool debug_mode;

	void (*on_completed_function)(String& result_content);

	bool sent = false;
	unsigned long sent_timestamp = 0;
	bool on_completed_called = false;

	bool first_process_call = true;
	unsigned long first_process_call_timestamp = 0;

	String rx_buffer = "";

	void serialReadToRxBuffer(size_t max_bytes_per_call = 32);
	void resetRxBuffer();
	virtual void resetState();
};

#endif 