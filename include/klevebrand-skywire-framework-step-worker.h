#ifndef KLEVEBRAND_SKYWIRE_STEP_WORKER_H
#define KLEVEBRAND_SKYWIRE_STEP_WORKER_H

#include "Arduino.h"
#include "klevebrand-skywire-framework.h"
#include "klevebrand-skywire-framework-step.h"

class SkywireStepWorker 
{
public:
    SkywireStepWorker(HardwareSerial& skywire_serial, bool debug_mode, int step_count) : 
        step_count(step_count),
        skywire(skywire_serial, debug_mode) {} 

	bool start();
	void run();
	void resetState();

    int step_count = 0;

	Skywire skywire;

	SkywireStep **steps;

	unsigned long last_run_timestamp = 0;
};

#endif // KLEVEBRAND_SKYWIRE_STEP_WORKER_H
