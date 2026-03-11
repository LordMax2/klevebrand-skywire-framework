#include "skywire-command-worker.h"

SkywireCommandWorker::SkywireCommandWorker(HardwareSerial *skywire_serial, bool debug_mode, const unsigned long timeout_milliseconds, const int step_count)
	: step_count(step_count), skywire(skywire_serial), timeout_milliseconds(timeout_milliseconds)
{
}

void SkywireCommandWorker::reset() const {
	for (int i = 0; i < step_count; i++)
	{
		steps[i]->reset();
	}
}

bool SkywireCommandWorker::run()
{
	const auto step = steps[step_cursor_index];

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
		const auto rx_buffer = SkywireCommand::getRxBuffer();

		if (sent_timestamp != 0 && millis() - sent_timestamp > timeout_milliseconds)
		{
			Serial.println("Skywire command step: " + String(step->command) + ", after " + timeout_milliseconds + "ms, restarting startup sequence." + " Sent timestamp: " + sent_timestamp + ", current timestamp: " + millis());
			Serial.println("rx_buffer at timeout: [" + String(rx_buffer) + "]");

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