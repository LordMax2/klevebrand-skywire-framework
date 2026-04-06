#include "Arduino.h"
#include "skywire-command-startup-worker.h"
#include "skywire-command-worker.h"
#include "skywire-command-at.h"
#include "skywire-command-httpsnd.h"
#include "skywire-command-httpring.h"
#include "skywire-command-httprcv.h"
#include "skywire-command-disable-echo.h"
#include "skywire-command-set-hologram-apn.h"
#include "skywire-command-network-connect.h"
#include "skywire-command-enable-packet-data.h"
#include "skywire-command-enable-gps.h"
#include "skywire-command.h"
#include "skywire-tcp-gps-step-worker.h"

void setup() {
    Serial.begin(115200);

    Serial.println("Starting Skywire modem...");

    SkywireCommandStartupWorker startupWorker(&Serial3, false);
    SkywireTcpGpsStepWorker tcpGpsWorker(&Serial3, "flightcontroltower.klevebrand.se", 13000, 20000, true);

    Serial3.begin(true);

    while (!startupWorker.run()) {
        // Do nothing until startup is completed.
    }

    Serial.println("Startup completed!");

    while (true) {
        String payload_to_send = "1;1337;true;1;2;3;99;120.5;59.8586;17.6389;42.5;1013.2;2;7";
        tcpGpsWorker.setPayloadToSend(payload_to_send.c_str());
        tcpGpsWorker.run();
    }
}

void loop() {
}
