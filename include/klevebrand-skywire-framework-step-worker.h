#ifndef KLEVEBRAND_SKYWIRE_HTTP_STEPPER_CLIENT_H
#define KLEVEBRAND_SKYWIRE_HTTP_STEPPER_CLIENT_H

#include "Arduino.h"
#include "klevebrand-skywire-framework.h"
#include "klevebrand-skywire-framework-step.h"

class SkywireStepWorker 
{
public:
	bool start();
	void run();
	void resetState();

    int step_count = 0;

private:
	Skywire _skywire;

	SkywireStep **_steps;
};

#endif // KLEVEBRAND_SKYWIRE_HTTP_STEPPER_CLIENT_H
