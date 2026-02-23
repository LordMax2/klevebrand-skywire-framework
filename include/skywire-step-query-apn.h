#ifndef SKYWIRE_STEP_QUERY_APN_H
#define SKYWIRE_STEP_QUERY_APN_H

#include "skywire-step.h"

class QueryApnSkywireStep : public SkywireStep {
public:
    QueryApnSkywireStep(Skywire &skywire, bool debug_mode, void (*on_completed_function)(String& result_content)) : SkywireStep(skywire, "AT+CGDCONT?\r", debug_mode, on_completed_function) {}
};

#endif // SKYWIRE_STEP_QUERY_APN_H
