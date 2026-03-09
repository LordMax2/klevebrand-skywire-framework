#include "skywire-command-worker.h"

SkywireCommandWorker::SkywireCommandWorker(HardwareSerial *skywire_serial, bool debug_mode, unsigned long timeout_milliseconds, int step_count)
	: step_count(step_count), skywire(skywire_serial), timeout_milliseconds(timeout_milliseconds)
{
}

void SkywireCommandWorker::reset()
{
	for (int i = 0; i < step_count; i++)
	{
		steps[i]->reset();
	}
}

bool SkywireCommandWorker::run()
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
				Serial.println("rx_buffer at timeout: [" + rx_buffer + "], previous step rx_buffer: [" + (i > 0 ? steps[i - 1]->getRxBuffer() : "N/A") + "]");

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

	reset();

	return true;
}