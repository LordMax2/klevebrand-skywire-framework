#include "klevebrand-skywire-framework-step-worker.h"

void SkywireStepWorker::resetState()
{
	for (int i = 0; i < step_count; i++)
	{
		_steps[i]->resetState();
	}
}

bool SkywireStepWorker::start()
{
	_skywire.start();

	_skywire.enableGps();

	return true;
}

void SkywireStepWorker::run()
{
	for (int i = 0; i < step_count; i++)
	{
		SkywireResponseResult_t step_result = _steps[i]->process();

		if (!step_result.is_success)
		{
			break;
		}
	}

	bool all_completed = true;

	for (int i = 0; i < step_count; i++)
	{
		if (!_steps[i]->completed())
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
