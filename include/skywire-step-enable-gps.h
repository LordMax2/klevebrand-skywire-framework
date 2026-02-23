#ifndef SKYWIRE_STEP_ENABLE_GPS_H
#define SKYWIRE_STEP_ENABLE_GPS_H

#include "skywire-step.h"

class EnableGpsSkywireStep : public SkywireStep {
public:
    EnableGpsSkywireStep(Skywire &skywire, bool debug_mode, void (*on_completed_function)(String& result_content)) : SkywireStep(skywire, "AT$GPSP=1", debug_mode, on_completed_function) {}
};

#endif // SKYWIRE_STEP_ENABLE_GPS_H
