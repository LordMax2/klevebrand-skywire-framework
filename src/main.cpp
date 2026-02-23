#include "Arduino.h"
#include "skywire-step-startup-worker.h"
#include "skywire-step-worker.h"
#include "skywire-step-at.h"
#include "skywire-step-disable-echo.h"
#include "skywire-step-set-hologram-apn.h"
#include "skywire-step-network-connect.h"
#include "skywire-step-enable-packet-data.h"
#include "skywire-step-enable-gps.h"
#include "skywire-step.h"

void setup() {
    Serial.begin(115200);

    Serial.println("Starting Skywire modem...");

    SkywireStepStartupWorker startupWorker(Serial3, true);

    Serial3.begin(115200);

    while (!startupWorker.run())
    {
        // Do nothing until startup is completed.
    }

    Serial.println("Startup completed!");

    while(true)
    {
        // Do nothing, just keep the program running.
    }

}   

void loop() {

}