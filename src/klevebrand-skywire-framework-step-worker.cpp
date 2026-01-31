#include "klevebrand-skywire-framework-step-worker.h"

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
	if(millis() - last_run_timestamp < 100)
	{
		return;
	}

	for (int i = 0; i < step_count; i++)
	{
		SkywireResponseResult_t step_result = steps[i]->process();

		if (!step_result.is_success)
		{
			break;
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
