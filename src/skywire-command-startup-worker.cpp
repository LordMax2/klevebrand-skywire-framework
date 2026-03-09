#include "skywire-command-startup-worker.h"

SkywireCommandStartupWorker::SkywireCommandStartupWorker(HardwareSerial *skywire_serial, bool debug_mode)
    : SkywireCommandWorker(skywire_serial, debug_mode, 5000, STEP_COUNT)
{
    this->steps = new SkywireCommand *[STEP_COUNT];

    this->steps[0] = new AtSkywireCommand(skywire, debug_mode, onAtCommandCompleted);
    this->steps[1] = new SkywireCommand(skywire, "AT+CMEE=2", debug_mode, onAtCommandCompleted);
    this->steps[2] = new DisableEchoSkywireCommand(skywire, debug_mode, onDisableEchoCommandCompleted);
    this->steps[3] = new SetApnHologramSkywireCommand(skywire, debug_mode, onSetApnCommandCompleted);
    this->steps[4] = new NetworkConnectSkywireCommand(skywire, debug_mode, onNetworkConnectCommandCompleted);
    this->steps[5] = new EnablePacketDataSkywireCommand(skywire, debug_mode, onEnablePacketDataCommandCompleted);
    this->steps[6] = new EnableGpsSkywireCommand(skywire, debug_mode, onEnableGpsCommandCompleted);
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
    for (int i = 0; i < step_count; i++)
    {
        if (!steps[i]->completed())
        {
            const auto sent_timestamp = steps[i]->getSentTimestamp();
            const auto rx_buffer = steps[i]->getRxBuffer();

            if (sent_timestamp != 0 && millis() - sent_timestamp > timeout_milliseconds)
            {
                Serial.println("Skywire command step: " + String(steps[i]->command) + ", after " + timeout_milliseconds + "ms, restarting startup sequence." + " Sent timestamp: " + sent_timestamp + ", current timestamp: " + millis());
                Serial.println("rx_buffer at timeout: [" + String(rx_buffer) + "]");

                reset();

                skywire->begin(115200);
            }
            else
            {
                steps[i]->process();
            }

            return false;
        }
    }

    return true;
}
