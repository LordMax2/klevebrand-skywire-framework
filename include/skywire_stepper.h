#pragma once

#ifndef SKYWIRE_STEPPER_H
#define SKYWIRE_STEPPER_H

#include "Arduino.h"
#include "concept_skywire_command.h"
#include "skywire_at_engine.h"

enum class SkywireStepResult
{
    Processed,
    Advanced,
    Finished,
    TimedOut
};

template<uint8_t StepCount>
class SkywireStepper
{
public:
    explicit SkywireStepper(unsigned long timeout_milliseconds);

    bool isFinished() const;

    template<SkywireCommandConcept... Commands>
    SkywireStepResult stepCurrent(Commands &...commands);

    template<SkywireCommandConcept... Commands>
    void reset(Commands &...commands);

private:
    template<SkywireCommandConcept Command>
    SkywireStepResult step(Command &command);

    template<SkywireCommandConcept First, SkywireCommandConcept... Rest>
    SkywireStepResult stepCurrentFrom(uint8_t current, First &first, Rest &...rest);

    unsigned long _timeout_milliseconds;
    uint8_t _step_cursor_index;
};

template<uint8_t StepCount>
inline SkywireStepper<StepCount>::SkywireStepper(const unsigned long timeout_milliseconds)
    : _timeout_milliseconds(timeout_milliseconds),
      _step_cursor_index(0)
{
}

template<uint8_t StepCount>
inline bool SkywireStepper<StepCount>::isFinished() const
{
    return _step_cursor_index >= StepCount;
}

template<uint8_t StepCount>
template<SkywireCommandConcept Command>
SkywireStepResult SkywireStepper<StepCount>::step(Command &command)
{
    if (command.completed())
    {
        _step_cursor_index++;

        if (_step_cursor_index >= StepCount)
        {
            return SkywireStepResult::Finished;
        }

        return SkywireStepResult::Advanced;
    }

    const unsigned long sent_timestamp = command.getSentTimestamp();

    if (sent_timestamp != 0 && millis() - sent_timestamp > _timeout_milliseconds)
    {
        if (SkywireAtEngine::debugMode())
        {
            SkywireAtEngine::logStepTimeout(command.command(), _timeout_milliseconds, sent_timestamp);
        }

        return SkywireStepResult::TimedOut;
    }

    command.process();

    return SkywireStepResult::Processed;
}

template<uint8_t StepCount>
template<SkywireCommandConcept First, SkywireCommandConcept... Rest>
SkywireStepResult SkywireStepper<StepCount>::stepCurrentFrom(const uint8_t current, First &first, Rest &...rest)
{
    if (_step_cursor_index == current)
    {
        return step(first);
    }

    if constexpr (sizeof...(Rest) > 0)
    {
        return stepCurrentFrom(static_cast<uint8_t>(current + 1), rest...);
    }

    return SkywireStepResult::Finished;
}

template<uint8_t StepCount>
template<SkywireCommandConcept... Commands>
SkywireStepResult SkywireStepper<StepCount>::stepCurrent(Commands &...commands)
{
    static_assert(sizeof...(Commands) == StepCount, "command list size must match step count");

    return stepCurrentFrom(0, commands...);
}

template<uint8_t StepCount>
template<SkywireCommandConcept... Commands>
void SkywireStepper<StepCount>::reset(Commands &...commands)
{
    static_assert(sizeof...(Commands) == StepCount, "command list size must match step count");

    (commands.reset(), ...);
    _step_cursor_index = 0;
}

#endif
