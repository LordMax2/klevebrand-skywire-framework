#include "skywire-command-startup-worker.h"

SkywireCommandStartupWorker::SkywireCommandStartupWorker(HardwareSerial *skywire_serial, bool debug_mode)
    : SkywireCommandWorker(skywire_serial, debug_mode, 5000, STARTUP_STEP_COUNT),
      at_command(skywire_serial, debug_mode, nullptr),
      cmee_command(skywire_serial, "AT+CMEE=2", debug_mode, nullptr),
      disable_echo_command(skywire_serial, debug_mode, nullptr),
      flow_control_command(skywire_serial, "AT&K0", debug_mode, nullptr),
      interface_control_command(skywire_serial, "AT+IFC=0,0", debug_mode, nullptr),
      set_apn_command(skywire_serial, debug_mode, nullptr),
      network_connect_command(skywire_serial, debug_mode, nullptr),
      enable_packet_data_command(skywire_serial, debug_mode, nullptr),
      enable_gps_command(skywire_serial, debug_mode, nullptr)
{
    this->steps[0] = &at_command;
    this->steps[1] = &cmee_command;
    this->steps[2] = &disable_echo_command;
    this->steps[3] = &flow_control_command;
    this->steps[4] = &interface_control_command;
    this->steps[5] = &set_apn_command;
    this->steps[6] = &network_connect_command;
    this->steps[7] = &enable_packet_data_command;
    this->steps[8] = &enable_gps_command;
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
            Serial.print(F("Skywire command step: "));
            Serial.print(step->command);
            Serial.print(F(", after "));
            Serial.print(timeout_milliseconds);
            Serial.print(F("ms, restarting startup sequence. Sent timestamp: "));
            Serial.print(sent_timestamp);
            Serial.print(F(", current timestamp: "));
            Serial.println(millis());
            Serial.print(F("rx_buffer at timeout: ["));
            Serial.print(rx_buffer);
            Serial.print(F("], previous step rx_buffer: ["));
            Serial.print(step_cursor_index > 0 ? steps[step_cursor_index - 1]->getRxBuffer() : "N/A");
            Serial.println(F("]"));

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
