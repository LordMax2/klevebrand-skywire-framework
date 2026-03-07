#ifndef SKYWIRE_STEP_HTTPSND_H
#define SKYWIRE_STEP_HTTPSND_H

#include "skywire-command.h"

class HttpSndSkywireCommand : public SkywireCommand {
public:
    HttpSndSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content));

    virtual SkywireResponseResult_t process() override;
};

#endif 