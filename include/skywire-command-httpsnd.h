#ifndef SKYWIRE_STEP_HTTPSND_H
#define SKYWIRE_STEP_HTTPSND_H

#include "skywire-command.h"

class HttpSndSkywireCommand : public SkywireCommand {
public:
    HttpSndSkywireCommand(HardwareSerial* skywire, bool debug_mode, String path, void (*on_completed_function)(String &result_content));

    SkywireResponseResult_t process() override;
    bool completed() override;

    bool arrowsReceived();
    void setPayload(String payload);
    String getPayload();
    void resetState() override;

private:
    String payload = "";
    bool payload_sent = false;
};

#endif 