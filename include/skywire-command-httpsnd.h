#ifndef SKYWIRE_STEP_HTTPSND_H
#define SKYWIRE_STEP_HTTPSND_H

#include "skywire-command.h"

#define HTTP_SND_PATH_SIZE 64

class HttpSndSkywireCommand : public SkywireCommand
{
public:
    HttpSndSkywireCommand(HardwareSerial *skywire, bool debug_mode, const char path[HTTP_SND_PATH_SIZE], OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process() override;
    bool completed() override;

    bool arrowsReceived();
    void setPayload(char payload[128]);
    char* getPayload();
    void reset() override;

private:
    char payload[128]{};
    char path[HTTP_SND_PATH_SIZE]{};
    bool payload_sent = false;
};

#endif