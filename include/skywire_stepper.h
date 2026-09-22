#pragma once

#ifndef SKYWIRE_STEPPER_H
#define SKYWIRE_STEPPER_H

#include "Arduino.h"
#include "concept_skywire_command.h"
#include "skywire_at_engine.h"
#include "skywire_step_result.h"

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

#include "skywire_stepper.ipp"

#endif
