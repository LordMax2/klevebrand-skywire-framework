#include "klevebrand-skywire-framework.h"
#include <SoftwareSerial.h>

void Skywire::start() {
    Skywire::skywire = SoftwareSerial(SKYWIRE_RX_PIN, SKYWIRE_TX_PIN);
}

void Skywire::send() { 

}

void Skywire::stop() { 

}