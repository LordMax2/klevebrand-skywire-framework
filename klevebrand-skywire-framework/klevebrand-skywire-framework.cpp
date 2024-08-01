#include "klevebrand-skywire-framework.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

void Skywire::start() {
    Skywire::skywire = SoftwareSerial(SKYWIRE_RX_PIN, SKYWIRE_TX_PIN);

    delay(5000);

    skywire.write("AT");

    while(skywire.available()) {
        String data = skywire.readString();
    }
}

void Skywire::send() { 

}

void Skywire::stop() { 

}