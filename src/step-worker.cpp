#include "skywire-step-worker.h"

void SkywireStepWorker::resetState()
{
	for (int i = 0; i < step_count; i++)
	{
		steps[i]->resetState();
	}
}

bool SkywireStepWorker::start()
{
	skywire.start();

	skywire.enableGps();

	return true;
}

void SkywireStepWorker::run()
{
	for (int i = 0; i < step_count; i++)
	{
		SkywireResponseResult_t step_result = steps[i]->process();

		// Dont go further if not successful.
		// Also, add do not go to the next step until 100ms has passed
		if (!step_result.is_success || millis() - steps[i]->sent_timestamp < 100)
		{
			break;
		}

		// If timeout on a step, force reset everything and start over
		if (millis() - steps[i]->sent_timestamp > timeout_milliseconds)
		{
			resetState();
		}
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
	}
}