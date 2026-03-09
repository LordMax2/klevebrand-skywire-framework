#ifndef SKYWIRE_STEP_HTTP_RING_H
#define SKYWIRE_STEP_HTTP_RING_H

#include "Arduino.h"
#include "skywire-command.h"

class HttpRingSkywireCommand : public SkywireCommand
{
public:
	HttpRingSkywireCommand(HardwareSerial *skywire, bool debug_mode, void (*on_completed_function)(String &result_content));
	SkywireResponseResult_t process() override;

	bool okReceived() override;

	bool completed() override;
};

#endif