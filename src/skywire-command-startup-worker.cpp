#include "skywire-command-startup-worker.h"

SkywireCommandStartupWorker::SkywireCommandStartupWorker(HardwareSerial* skywire_serial, bool debug_mode)
    : SkywireCommandWorker(skywire_serial, debug_mode, 5000, STEP_COUNT)
{
    this->steps = new SkywireCommand *[STEP_COUNT];

    this->steps[0] = new AtSkywireCommand(skywire, debug_mode, onAtCommandCompleted);
    this->steps[1] = new DisableEchoSkywireCommand(skywire, debug_mode, onDisableEchoCommandCompleted);
    this->steps[2] = new SetApnHologramSkywireCommand(skywire, debug_mode, onSetApnCommandCompleted);
    this->steps[3] = new NetworkConnectSkywireCommand(skywire, debug_mode, onNetworkConnectCommandCompleted);
    this->steps[4] = new EnablePacketDataSkywireCommand(skywire, debug_mode, onEnablePacketDataCommandCompleted);
    this->steps[5] = new EnableGpsSkywireCommand(skywire, debug_mode, onEnableGpsCommandCompleted);
}

void SkywireCommandStartupWorker::onAtCommandCompleted(String &result_content)
{
    Serial.println("AT command completed with result: " + result_content);
}

void SkywireCommandStartupWorker::onDisableEchoCommandCompleted(String &result_content)
{
    Serial.println("Disable echo command completed with result: " + result_content);
}

void SkywireCommandStartupWorker::onSetApnCommandCompleted(String &result_content)
{
    Serial.println("Set APN command completed with result: " + result_content);
}

void SkywireCommandStartupWorker::onNetworkConnectCommandCompleted(String &result_content)
{
    Serial.println("Network connect command completed with result: " + result_content);
}

void SkywireCommandStartupWorker::onEnablePacketDataCommandCompleted(String &result_content)
{
    Serial.println("Enable packet data command completed with result: " + result_content);
}

void SkywireCommandStartupWorker::onEnableGpsCommandCompleted(String &result_content)
{
    Serial.println("Enable GPS command completed with result: " + result_content);
}

bool SkywireCommandStartupWorker::run()
{
    skywire->begin(115200);

    for (int i = 0; i < step_count; i++)
    {
        // If timeout on a step, force reset everything and start over
        if (steps[i]->sent && millis() - steps[i]->sent_timestamp > timeout_milliseconds && !steps[i]->completed())
        {
            Serial.println("Skywire command step: " + String(steps[i]->command) + ", after " + timeout_milliseconds + "ms, restarting startup sequence.");

            skywire->end();
            skywire->begin(115200);

            resetState();
        }

        steps[i]->process();
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
