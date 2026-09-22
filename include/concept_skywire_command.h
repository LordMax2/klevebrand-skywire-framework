#pragma once

#ifndef CONCEPT_SKYWIRE_COMMAND_H
#define CONCEPT_SKYWIRE_COMMAND_H

#include "concepts.h"
#include "skywire-response-result.h"

template<typename Command>
concept SkywireCommandConcept = requires(Command& command, const Command& const_command)
{
    { command.process() } -> same_as<SkywireResponseResult_t>;

    { command.reset() } -> same_as<void>;

    { const_command.completed() } -> same_as<bool>;

    { const_command.getLastSendTimestamp() } -> same_as<unsigned long>;

    { const_command.command() } -> same_as<const __FlashStringHelper *>;
};

#endif
