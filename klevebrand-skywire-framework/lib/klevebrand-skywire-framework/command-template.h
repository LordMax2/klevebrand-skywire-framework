#ifndef COMMAND_TEMPLATE_H
#define COMMAND_TEMPLATE_H

#include "klevebrand-skywire-framework.h"
#include "Arduino.h"

class CommandTemplate
{
public:
    virtual bool Execute(Skywire *skywire)
    {
        return false;
    };
    virtual bool ValidateResponse(Skywire *skywire)
    {
        return skywire->waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &responseOkSerialPrint);
    };

private:
    static bool responseOkSerialPrint(String responseContent)
    {
        Serial.println(responseContent);
        return true;
    }
};

#endif // COMMAND_TEMPLATE_H