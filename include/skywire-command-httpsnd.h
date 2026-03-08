#ifndef SKYWIRE_STEP_HTTPSND_H
#define SKYWIRE_STEP_HTTPSND_H

#include "skywire-command.h"

class HttpSndSkywireCommand : public SkywireCommand {
public:
    HttpSndSkywireCommand(HardwareSerial* skywire, bool debug_mode, String path, void (*on_completed_function)(String &result_content));

    SkywireResponseResult_t process() override;

    bool completed() override;

    bool arrowsReceived()
    {
        return rx_buffer.indexOf(">>>") != -1 || millis() - sent_timestamp > 200;
    }

    String payload = "";

    void setPayload(String payload)
    {
        this->payload = payload;
    }

    String getPayload()
    {
        return payload;
    }

    bool payload_sent = false;

    void resetState() override
    {
        SkywireCommand::resetState();
        payload_sent = false;
    }
};

#endif 