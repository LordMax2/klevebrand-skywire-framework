#ifndef SKYWIRE_STEP_WORKER_H
#define SKYWIRE_STEP_WORKER_H

#include "Arduino.h"
#include "skywire-command.h"

class SkywireCommandWorker 
{
public:
    SkywireCommandWorker(HardwareSerial* skywire_serial, bool debug_mode, unsigned long timeout_milliseconds, int step_count) : 
        step_count(step_count),
        skywire(skywire_serial),
		timeout_milliseconds(timeout_milliseconds) {} 

	virtual bool run();
	void resetState();

    int step_count = 0;

	HardwareSerial* skywire;

	SkywireCommand **steps;

	unsigned long last_run_timestamp = 0;
	unsigned long timeout_milliseconds = 0;
};

#endif // SKYWIRE_STEP_WORKER_H
