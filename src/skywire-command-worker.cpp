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
	auto step = steps[step_cursor_index];

	if (step->completed())
	{
		step_cursor_index++;

		if (step_cursor_index >= step_count)
		{
			step_cursor_index = 0;

			reset();

			return true;
		}
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