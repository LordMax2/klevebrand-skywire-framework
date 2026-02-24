#ifndef SKYWIRE_STEP_STARTUP_WORKER_H
#define SKYWIRE_STEP_STARTUP_WORKER_H

#include "Arduino.h"
#include "skywire-command-worker.h"
#include "skywire-command-at.h"
#include "skywire-command-disable-echo.h"
#include "skywire-command-set-hologram-apn.h"
#include "skywire-command-network-connect.h"
#include "skywire-command-enable-packet-data.h"
#include "skywire-command-enable-gps.h"

#define STEP_COUNT 6

class SkywireCommandStartupWorker : public SkywireCommandWorker
{
public:
    SkywireCommandStartupWorker(
        HardwareSerial* skywire_serial,
        bool debug_mode = false) : SkywireCommandWorker(skywire_serial, debug_mode, 5000, STEP_COUNT)
    {
        this->steps = new SkywireCommand *[STEP_COUNT];

        this->steps[0] = new AtSkywireCommand(skywire, debug_mode, onAtCommandCompleted);
        this->steps[1] = new DisableEchoSkywireCommand(skywire, debug_mode, onDisableEchoCommandCompleted);
        this->steps[2] = new SetApnHologramSkywireCommand(skywire, debug_mode, onSetApnCommandCompleted);
        this->steps[3] = new NetworkConnectSkywireCommand(skywire, debug_mode, onNetworkConnectCommandCompleted);
        this->steps[4] = new EnablePacketDataSkywireCommand(skywire, debug_mode, onEnablePacketDataCommandCompleted);
        this->steps[5] = new EnableGpsSkywireCommand(skywire, debug_mode, onEnableGpsCommandCompleted);
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
                Serial.println("Skywire command step: " + String(steps[i]->command) + ", after " + timeout_milliseconds + "ms, restarting startup sequence.");

                resetState();
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