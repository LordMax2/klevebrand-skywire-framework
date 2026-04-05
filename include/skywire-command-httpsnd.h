#ifndef SKYWIRE_STEP_HTTPSND_H
#define SKYWIRE_STEP_HTTPSND_H

#include "skywire-command.h"

#define HTTP_SND_PATH_SIZE 64
#define HTTP_SND_PAYLOAD_TO_SEND_SIZE 128

class HttpSndSkywireCommand : public SkywireCommand
{
public:
    HttpSndSkywireCommand(HardwareSerial *skywire, bool debug_mode, const char path[HTTP_SND_PATH_SIZE], OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process() override;
    bool completed() override;

    bool arrowsReceived() const;
    void setPayload(char payload_to_send[HTTP_SND_PAYLOAD_TO_SEND_SIZE]);
    char* getPayload();
    void reset() override;

private:
    char payload[HTTP_SND_PAYLOAD_TO_SEND_SIZE]{};
    char path[HTTP_SND_PATH_SIZE]{};
    bool payload_sent = false;
};

#endif