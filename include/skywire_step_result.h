#pragma once

#ifndef SKYWIRE_STEP_RESULT_H
#define SKYWIRE_STEP_RESULT_H

enum class SkywireStepResult
{
    SequenceStillRunning,
    SequenceCompleted,
    StepTimedOut
};

#endif
