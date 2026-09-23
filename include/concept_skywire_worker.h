#pragma once

#ifndef CONCEPT_SKYWIRE_WORKER_H
#define CONCEPT_SKYWIRE_WORKER_H

#include "concepts.h"

template<typename Worker>
concept SkywireWorkerConcept = requires(Worker& worker)
{
    { worker.run() } -> same_as<bool>;

    { worker.reset() } -> same_as<void>;
};

#endif
