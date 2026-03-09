#ifndef SKYWIRE_STEP_HTTP_RCV_H
#define SKYWIRE_STEP_HTTP_RCV_H

#include "Arduino.h"
#include "skywire-command.h"

class HttpRcvSkywireCommand : public SkywireCommand
{
public:
    HttpRcvSkywireCommand(HardwareSerial *skywire, bool debug_mode, void (*on_completed_function)(String &result_content));
    SkywireResponseResult_t process() override;

    bool okReceived() override;

private:
    unsigned long timestamp_milliseconds = 0;
};

#endif