#pragma once

#ifndef SKYWIRE_STEPPER_H
#define SKYWIRE_STEPPER_H

#include "Arduino.h"
#include "concept_skywire_command.h"
#include "skywire_at_engine.h"

enum class SkywireStepResult
{
    SequenceStillRunning,
    SequenceCompleted,
    StepTimedOut
};

template<uint8_t StepCount>
class SkywireStepper
{
public:
    explicit SkywireStepper(unsigned long timeout_milliseconds);

    bool hasCompletedAllCommands() const;

    template<SkywireCommandConcept... Commands>
    SkywireStepResult stepCurrent(Commands &...commands);

    template<SkywireCommandConcept... Commands>
    void reset(Commands &...commands);

private:
    template<SkywireCommandConcept Command>
    SkywireStepResult stepCommand(Command &command);

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
inline bool SkywireStepper<StepCount>::hasCompletedAllCommands() const
{
    return _step_cursor_index >= StepCount;
}

template<uint8_t StepCount>
template<SkywireCommandConcept Command>
SkywireStepResult SkywireStepper<StepCount>::stepCommand(Command &command)
{
    if (command.completed())
    {
        _step_cursor_index++;

        if (_step_cursor_index >= StepCount)
        {
            return SkywireStepResult::SequenceCompleted;
        }

        return SkywireStepResult::SequenceStillRunning;
    }

    const unsigned long sent_timestamp = command.getSentTimestamp();

    if (sent_timestamp != 0 && millis() - sent_timestamp > _timeout_milliseconds)
    {
        if (SkywireAtEngine::debugMode())
        {
            SkywireAtEngine::logStepTimeout(command.command(), _timeout_milliseconds, sent_timestamp);
        }

        return SkywireStepResult::StepTimedOut;
    }

    command.process();

    return SkywireStepResult::SequenceStillRunning;
}

template<uint8_t StepCount>
template<SkywireCommandConcept... Commands>
SkywireStepResult SkywireStepper<StepCount>::stepCurrent(Commands &...commands)
{
    static_assert(sizeof...(Commands) == StepCount, "command list size must match step count");

    SkywireStepResult result = SkywireStepResult::SequenceCompleted;
    uint8_t command_index = 0;

    const auto step_matching_command = [&](auto &command) -> bool
    {
        if (command_index == _step_cursor_index)
        {
            result = stepCommand(command);

            return true;
        }

        command_index++;

        return false;
    };

    (step_matching_command(commands) || ...);

    return result;
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
