#ifndef SKYWIRE_STEP_H
#define SKYWIRE_STEP_H

#include "Arduino.h"
#include "skywire-response-result.h"

class SkywireCommand
{
private:
	bool _sent = false;
	unsigned long _sent_timestamp = 0;
	bool _on_completed_called = false;

	bool _first_process_call = true;
	unsigned long _first_process_call_timestamp = 0;

	String _rx_buffer = "";

public:
	SkywireCommand(HardwareSerial* skywire, String command, bool debug_mode, void (*on_completed_function)(String& result_content));

	virtual SkywireResponseResult_t process();
	virtual bool okReceived();
	virtual bool completed();
	virtual void reset();

	HardwareSerial* skywire;
	String command;
	bool debug_mode;

	void (*on_completed_function)(String& result_content);

	void setSent(bool sent);
	bool isSent();
	unsigned long getSentTimestamp();

	void setOnCompletedCalled(bool on_completed_called);
	bool isOnCompletedCalled();

	void setFirstProcessCall();
	bool isFirstProcessCalled();
	unsigned long getFirstProcessCallTimestamp();

	String getRxBuffer();
	void appendToRxBuffer(char c);
	void serialReadToRxBuffer();
	void resetRxBuffer();

};

#endif 