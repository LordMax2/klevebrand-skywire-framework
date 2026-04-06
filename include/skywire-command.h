#ifndef SKYWIRE_STEP_H
#define SKYWIRE_STEP_H

#include "Arduino.h"
#include "skywire-response-result.h"

#define COMMAND_SIZE 128
#define SKYWIRE_RX_BUFFER_SIZE 512

typedef void (*OnCompletedFunction)(char *result_content);

class SkywireCommand
{
private:
	bool _sent = false;
	unsigned long _sent_timestamp = 0;
	bool _on_completed_called = false;

	bool _first_process_call = true;
	unsigned long _first_process_call_timestamp = 0;

	size_t _rx_buffer_cursor_index = 0;
	static char _rx_buffer[SKYWIRE_RX_BUFFER_SIZE];


public:
	virtual ~SkywireCommand() = default;

	SkywireCommand(HardwareSerial *skywire, const char command[COMMAND_SIZE], bool debug_mode, OnCompletedFunction on_completed_function);

	virtual SkywireResponseResult_t process();
	virtual bool okReceived();
	virtual bool completed();
	virtual void reset();

	HardwareSerial *skywire;
	char command[COMMAND_SIZE]{};
	bool debug_mode;

	OnCompletedFunction on_completed_function;

	void setSent(bool sent);
	bool isSent() const;
	unsigned long getSentTimestamp() const;

	void setOnCompletedCalled(bool on_completed_called);
	bool isOnCompletedCalled() const;

	void setFirstProcessCall();
	bool isFirstProcessCalled() const;
	unsigned long getFirstProcessCallTimestamp() const;

	static char* getRxBuffer();
	void appendToRxBuffer(char c);
	void serialReadToRxBuffer();
	void resetRxBuffer();

	void setCompleted(bool completed);
	bool _is_completed = false;
};

#endif
