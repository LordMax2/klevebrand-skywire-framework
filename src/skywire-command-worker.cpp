#include "skywire-command-worker.h"

SkywireCommandWorker::SkywireCommandWorker(HardwareSerial *skywire_serial, bool debug_mode, const unsigned long timeout_milliseconds, const int step_count)
	: step_count(step_count), skywire(skywire_serial), timeout_milliseconds(timeout_milliseconds)
{
}

void SkywireCommandWorker::reset() {
	for (int i = 0; i < step_count; i++)
	{
		steps[i]->reset();
	}

	step_cursor_index = 0;
}

bool SkywireCommandWorker::run()
{
	const auto step = steps[step_cursor_index];

	if (step->completed())
	{
		step_cursor_index++;

		if (step_cursor_index >= step_count)
		{
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
