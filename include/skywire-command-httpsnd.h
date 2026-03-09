#ifndef SKYWIRE_STEP_HTTPSND_H
#define SKYWIRE_STEP_HTTPSND_H

#include "skywire-command.h"

class HttpSndSkywireCommand : public SkywireCommand
{
public:
    HttpSndSkywireCommand(HardwareSerial *skywire, bool debug_mode, char path[256], OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process() override;
    bool completed() override;

    bool arrowsReceived();
    void setPayload(char payload[1024]);
    char* getPayload();
    void reset() override;

private:
    char payload[1024];
    char path[256];
    bool payload_sent = false;
};

#endif