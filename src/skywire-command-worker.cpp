#include "skywire-command-worker.h"

SkywireCommandWorker::SkywireCommandWorker(HardwareSerial* skywire_serial, bool debug_mode, unsigned long timeout_milliseconds, int step_count)
	: step_count(step_count), skywire(skywire_serial), timeout_milliseconds(timeout_milliseconds)
{
}

void SkywireCommandWorker::resetState()
{
	for (int i = 0; i < step_count; i++)
	{
		steps[i]->resetState();
	}
}

bool SkywireCommandWorker::run()
{
	// Process only the first incomplete step. All steps share the same
	// HardwareSerial, so calling process() on multiple steps per run()
	// causes their serialReadToRxBuffer() calls to race on the same UART
	// RX buffer and steal bytes from each other, producing garbled output.
	for (int i = 0; i < step_count; i++)
	{
		if (!steps[i]->completed())
		{
			if (steps[i]->sent_timestamp != 0 &&
				millis() - steps[i]->sent_timestamp > timeout_milliseconds)
			{
				Serial.println("Skywire command step: " + String(steps[i]->command) + ", after " + timeout_milliseconds + "ms, restarting startup sequence." + " Sent timestamp: " + steps[i]->sent_timestamp + ", current timestamp: " + millis());
				Serial.println("rx_buffer at timeout: [" + steps[i]->rx_buffer + "], previous step rx_buffer: [" + (i > 0 ? steps[i - 1]->rx_buffer : "N/A") + "]");

				resetState();

				skywire->begin(115200);
			}
			else
			{
				steps[i]->process();
			}

			return false;
		}
	}

	resetState();

	return true;
}