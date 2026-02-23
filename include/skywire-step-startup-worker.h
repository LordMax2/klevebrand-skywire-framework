#ifndef SKYWIRE_STEP_STARTUP_WORKER_H
#define SKYWIRE_STEP_STARTUP_WORKER_H

#include "Arduino.h"
#include "skywire-step-worker.h"
#include "skywire-step-at.h"
#include "skywire-step-disable-echo.h"
#include "skywire-step-set-hologram-apn.h"
#include "skywire-step-network-connect.h"
#include "skywire-step-enable-packet-data.h"
#include "skywire-step-enable-gps.h"

#define STEP_COUNT 6

class SkywireStepStartupWorker : public SkywireStepWorker
{
public:
    SkywireStepStartupWorker(
        HardwareSerial &skywire_serial,
        bool debug_mode = false) : SkywireStepWorker(skywire_serial, debug_mode, 5000, STEP_COUNT)
    {
        this->steps = new SkywireStep *[STEP_COUNT];

        this->steps[0] = new AtSkywireStep(skywire, debug_mode, onAtCommandCompleted);
        this->steps[1] = new DisableEchoSkywireStep(skywire, debug_mode, onDisableEchoCommandCompleted);
        this->steps[2] = new SetApnHologramSkywireStep(skywire, debug_mode, onSetApnCommandCompleted);
        this->steps[3] = new NetworkConnectSkywireStep(skywire, debug_mode, onNetworkConnectCommandCompleted);
        this->steps[4] = new EnablePacketDataSkywireStep(skywire, debug_mode, onEnablePacketDataCommandCompleted);
        this->steps[5] = new EnableGpsSkywireStep(skywire, debug_mode, onEnableGpsCommandCompleted);
    }

    static void onAtCommandCompleted(String &result_content)
    {
        Serial.println("AT command completed with result: " + result_content);
    }

    static void onDisableEchoCommandCompleted(String &result_content)
    {
        Serial.println("Disable echo command completed with result: " + result_content);
    }

    static void onSetApnCommandCompleted(String &result_content)
    {
        Serial.println("Set APN command completed with result: " + result_content);
    }


    static void onNetworkConnectCommandCompleted(String &result_content)
    {
        Serial.println("Network connect command completed with result: " + result_content);
    }

    static void onEnablePacketDataCommandCompleted(String &result_content)
    {
        Serial.println("Enable packet data command completed with result: " + result_content);
    }

    static void onEnableGpsCommandCompleted(String &result_content)
    {
        Serial.println("Enable GPS command completed with result: " + result_content);
    }

    bool run() override
    {
        for (int i = 0; i < step_count; i++)
        {
            SkywireResponseResult_t step_result = steps[i]->process();

            // Dont go further if not successful.
            // Also, add do not go to the next step until 100ms has passed
            if (!step_result.is_success || millis() - steps[i]->sent_timestamp < 100)
            {
                break;
            }

            // If timeout on a step, force reset everything and start over
            if (millis() - steps[i]->sent_timestamp > timeout_milliseconds)
            {
                //resetState();
            }
        }

        bool all_completed = true;

        for (int i = 0; i < step_count; i++)
        {
            if (!steps[i]->completed())
            {
                all_completed = false;
                break;
            }
        }

        return all_completed;
    }
};

#endif // SKYWIRE_STEP_STARTUP_WORKER_H