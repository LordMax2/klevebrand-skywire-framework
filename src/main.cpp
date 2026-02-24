#include "Arduino.h"
#include "skywire-command-startup-worker.h"
#include "skywire-command-worker.h"
#include "skywire-command-at.h"
#include "skywire-command-disable-echo.h"
#include "skywire-command-set-hologram-apn.h"
#include "skywire-command-network-connect.h"
#include "skywire-command-enable-packet-data.h"
#include "skywire-command-enable-gps.h"
#include "skywire-command.h"

void setup() {
    /*
    Serial.begin(115200);

    Serial.println("Starting Skywire modem...");

    SkywireCommandStartupWorker startupWorker(&Serial3, true);

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
    */
}   

void loop() {

}