#ifndef SKYWIRE_STEP_HTTPSND_H
#define SKYWIRE_STEP_HTTPSND_H

#include "skywire-command.h"

class HttpSndSkywireCommand : public SkywireCommand
{
public:
    HttpSndSkywireCommand(HardwareSerial *skywire, bool debug_mode, const char path[32], OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process() override;
    bool completed() override;

    bool arrowsReceived();
    void setPayload(char payload[128]);
    char* getPayload();
    void reset() override;

private:
    char payload[128]{};
    char path[32]{};
    bool payload_sent = false;
};

#endif