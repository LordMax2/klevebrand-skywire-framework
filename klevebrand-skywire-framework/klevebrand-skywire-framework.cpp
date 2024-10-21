#include "Arduino.h"
#include "klevebrand-skywire-framework.h"

SoftwareSerial Skywire::skywireSerialChannel(SKYWIRE_RX_PIN, SKYWIRE_TX_PIN);

void Skywire::start() {
    Serial.println("Starting skywire modem...");

    skywireSerialChannel.begin(115200);  
    delay(1000);
    skywireSerialChannel.println("ATE0\r");
    delay(1000);
    skywireSerialChannel.println("AT\r");
}

void Skywire::stop() {
    
}
