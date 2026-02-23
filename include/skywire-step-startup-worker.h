#ifndef SKYWIRE_STEP_STARTUP_WORKER_H
#define SKYWIRE_STEP_STARTUP_WORKER_H

#include "Arduino.h"
#include "skywire-step-worker.h"
#include "skywire-step-at.h"
#include "skywire-step-disable-echo.h"

#define STEP_COUNT 5

class SkywireStepStartupWorker : public SkywireStepWorker
{
public:
    SkywireStepStartupWorker(
        HardwareSerial &skywire_serial,
        bool debug_mode = false) : SkywireStepWorker(skywire_serial, debug_mode, 5000)
    {
        this->steps = new SkywireStep *[STEP_COUNT];

        this->steps[0] = new AtSkywireStep(skywire, debug_mode, onAtCommandCompleted);
        this->steps[1] = new DisableEchoSkywireStep(skywire, debug_mode, onDisableEchoCommandCompleted);
    }

    static void onAtCommandCompleted(String &result_content)
    {
        Serial.println("AT command completed with result: " + result_content);
    }

    static void onDisableEchoCommandCompleted(String &result_content)
    {
        Serial.println("Disable echo command completed with result: " + result_content);
    }
};

#endif // SKYWIRE_STEP_STARTUP_WORKER_H