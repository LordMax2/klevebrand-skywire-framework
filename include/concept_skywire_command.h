#pragma once

#ifndef CONCEPT_SKYWIRE_COMMAND_H
#define CONCEPT_SKYWIRE_COMMAND_H

#include "concepts.h"
#include "skywire-response-result.h"

template<typename T>
concept SkywireCommandConcept = requires(T& cmd, const T& ccmd)
{
    { cmd.process() } -> same_as<SkywireResponseResult_t>;
    { cmd.reset() } -> same_as<void>;
    { ccmd.completed() } -> same_as<bool>;
    { ccmd.getSentTimestamp() } -> same_as<unsigned long>;
    { ccmd.command() };
};

#endif
