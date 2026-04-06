#include "skywire-command-startup-worker.h"

SkywireCommandStartupWorker::SkywireCommandStartupWorker(HardwareSerial *skywire_serial, bool debug_mode)
    : SkywireCommandWorker(skywire_serial, debug_mode, 5000, STARTUP_STEP_COUNT)
{
    this->steps[0] = new AtSkywireCommand(skywire, debug_mode, onAtCommandCompleted);
    this->steps[1] = new SkywireCommand(skywire, "AT+CMEE=2", debug_mode, onAtCommandCompleted);
    this->steps[2] = new DisableEchoSkywireCommand(skywire, debug_mode, onDisableEchoCommandCompleted);
    this->steps[3] = new SkywireCommand(skywire, "AT&K0", true, onAtCommandCompleted);
    this->steps[4] = new SkywireCommand(skywire, "AT+IFC=0,0", true, onAtCommandCompleted);
    this->steps[5] = new SetApnHologramSkywireCommand(skywire, debug_mode, onSetApnCommandCompleted);
    this->steps[6] = new NetworkConnectSkywireCommand(skywire, debug_mode, onNetworkConnectCommandCompleted);
    this->steps[7] = new EnablePacketDataSkywireCommand(skywire, debug_mode, onEnablePacketDataCommandCompleted);
    this->steps[8] = new EnableGpsSkywireCommand(skywire, debug_mode, onEnableGpsCommandCompleted);
}

void SkywireCommandStartupWorker::onAtCommandCompleted(char *result_content)
{
    Serial.print("AT command completed with result: ");
    Serial.println(result_content);
}

void SkywireCommandStartupWorker::onDisableEchoCommandCompleted(char *result_content)
{
    Serial.print("Disable echo command completed with result: ");
    Serial.println(result_content);
}

void SkywireCommandStartupWorker::onSetApnCommandCompleted(char *result_content)
{
    Serial.print("Set APN command completed with result: ");
    Serial.println(result_content);
}

void SkywireCommandStartupWorker::onNetworkConnectCommandCompleted(char *result_content)
{
    Serial.print("Network connect command completed with result: ");
    Serial.println(result_content);
}

void SkywireCommandStartupWorker::onEnablePacketDataCommandCompleted(char *result_content)
{
    Serial.print("Enable packet data command completed with result: ");
    Serial.println(result_content);
}

void SkywireCommandStartupWorker::onEnableGpsCommandCompleted(char *result_content)
{
    Serial.print("Enable GPS command completed with result: ");
    Serial.println(result_content);
}

// The difference with this worker is that it only runs "once", it never "resets" the cursor when it is done.
bool SkywireCommandStartupWorker::run()
{
    if (step_cursor_index >= step_count)
    {
        return true;
    }

    auto step = steps[step_cursor_index];

    if (step->completed())
    {
        step_cursor_index++;
    }
    else
    {
        const auto sent_timestamp = step->getSentTimestamp();
        const auto rx_buffer = step->getRxBuffer();

        if (sent_timestamp != 0 && millis() - sent_timestamp > timeout_milliseconds)
        {
            Serial.println("Skywire command step: " + String(step->command) + ", after " + timeout_milliseconds + "ms, restarting startup sequence." + " Sent timestamp: " + sent_timestamp + ", current timestamp: " + millis());
            Serial.println("rx_buffer at timeout: [" + String(rx_buffer) + "], previous step rx_buffer: [" + (step_cursor_index > 0 ? String(steps[step_cursor_index - 1]->getRxBuffer()) : "N/A") + "]");

            reset();

            skywire->begin(115200);
        }
        else
        {
            step->process();
        }
    }

    return false;
}
