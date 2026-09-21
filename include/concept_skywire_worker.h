#pragma once

#ifndef CONCEPT_SKYWIRE_WORKER_H
#define CONCEPT_SKYWIRE_WORKER_H

#include "concepts.h"

template<typename T>
concept SkywireWorkerConcept = requires(T& w)
{
    { w.run() } -> same_as<bool>;
    { w.reset() } -> same_as<void>;
};

#endif
