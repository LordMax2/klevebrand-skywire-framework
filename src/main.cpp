#include "Arduino.h"
#include "skywire-command-startup-worker.h"
#include "skywire-command-tcp-gps-step-worker.h"

void setup()
{
    Serial.begin(115200);

    Serial.println(F("Starting Skywire modem..."));

    SkywireCommandStartupWorker startupWorker(&Serial3, false);
    SkywireTcpGpsStepWorker tcpGpsWorker(&Serial3, "flightcontroltower.klevebrand.se", 13000, 20000, false);

    Serial3.begin(115200);

    while (!startupWorker.run())
    {
    }

    Serial.println(F("Startup completed!"));

    while (true)
    {
        const char payload_to_send[] = "1;1337;true;1;2;3;99;120.5;59.8586;17.6389;42.5;1013.2;2;7";
        tcpGpsWorker.setPayloadToSend(payload_to_send);
        tcpGpsWorker.run();
    }
}

void loop()
{
}
