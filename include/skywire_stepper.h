#pragma once

#ifndef SKYWIRE_STEPPER_H
#define SKYWIRE_STEPPER_H

#include "Arduino.h"
#include "concept_skywire_command.h"
#include "skywire_at_engine.h"

enum class SkywireStepperTickResult
{
    Processed,
    Advanced,
    Finished,
    TimedOut
};

class SkywireStepper
{
public:
    SkywireStepper(unsigned long timeout_milliseconds, uint8_t step_count);

    uint8_t stepCursorIndex() const;
    bool isFinished() const;
    void resetCursor();

    template<SkywireCommandConcept Command>
    SkywireStepperTickResult tick(Command &cmd);

private:
    unsigned long _timeout_milliseconds;
    uint8_t _step_cursor_index;
    uint8_t _step_count;
};

inline SkywireStepper::SkywireStepper(const unsigned long timeout_milliseconds, const uint8_t step_count)
    : _timeout_milliseconds(timeout_milliseconds),
      _step_cursor_index(0),
      _step_count(step_count)
{
}

inline uint8_t SkywireStepper::stepCursorIndex() const
{
    return _step_cursor_index;
}

inline bool SkywireStepper::isFinished() const
{
    return _step_cursor_index >= _step_count;
}

inline void SkywireStepper::resetCursor()
{
    _step_cursor_index = 0;
}

template<SkywireCommandConcept Command>
SkywireStepperTickResult SkywireStepper::tick(Command &cmd)
{
    if (cmd.completed())
    {
        _step_cursor_index++;

        if (_step_cursor_index >= _step_count)
        {
            return SkywireStepperTickResult::Finished;
        }

        return SkywireStepperTickResult::Advanced;
    }

    const unsigned long sent_timestamp = cmd.getSentTimestamp();

    if (sent_timestamp != 0 && millis() - sent_timestamp > _timeout_milliseconds)
    {
        if (SkywireAtEngine::debugMode())
        {
            SkywireAtEngine::logStepTimeout(cmd.command(), _timeout_milliseconds, sent_timestamp);
        }

        return SkywireStepperTickResult::TimedOut;
    }

    cmd.process();

    return SkywireStepperTickResult::Processed;
}

#endif
