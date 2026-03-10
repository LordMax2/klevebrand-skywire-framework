#ifndef SKYWIRE_STEP_H
#define SKYWIRE_STEP_H

#include "Arduino.h"
#include "skywire-response-result.h"

typedef void (*OnCompletedFunction)(char result_content[512]);

class SkywireCommand
{
private:
	bool _sent = false;
	unsigned long _sent_timestamp = 0;
	bool _on_completed_called = false;

	bool _first_process_call = true;
	unsigned long _first_process_call_timestamp = 0;

	int _rx_buffer_cursor_index = 0;
	static char _rx_buffer[512];

	bool _is_completed = false;

public:
	SkywireCommand(HardwareSerial *skywire, const char command[256], bool debug_mode, OnCompletedFunction on_completed_function);

	virtual SkywireResponseResult_t process();
	virtual bool okReceived();
	virtual bool completed();
	virtual void reset();

	HardwareSerial *skywire;
	char command[256];
	bool debug_mode;

	OnCompletedFunction on_completed_function;

	void setSent(bool sent);
	bool isSent();
	unsigned long getSentTimestamp();

	void setOnCompletedCalled(bool on_completed_called);
	bool isOnCompletedCalled();

	void setFirstProcessCall();
	bool isFirstProcessCalled();
	unsigned long getFirstProcessCallTimestamp();

	char* getRxBuffer();
	void appendToRxBuffer(char c);
	void serialReadToRxBuffer();
	void resetRxBuffer();

	void setCompleted(bool completed);
};

#endif