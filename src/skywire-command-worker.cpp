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
	for (int i = 0; i < step_count; i++)
	{
        // If timeout on a step, force reset everything and start over
        if ((millis() - steps[i]->sent_timestamp > timeout_milliseconds && steps[i]->sent_timestamp != 0) && !steps[i]->completed())
        {
            Serial.println("Skywire command step: " + String(steps[i]->command) + ", after " + timeout_milliseconds + "ms, restarting startup sequence." + " Sent timestamp: " + steps[i]->sent_timestamp + ", current timestamp: " + millis());

            resetState();

            skywire->begin(115200);

            break;
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

	if (all_completed)
	{
		resetState();

		return true;
	}

	return false;
}