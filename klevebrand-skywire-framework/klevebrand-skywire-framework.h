#ifndef KLEVEBRAND_SKYWIRE_FRAMEWORK_H
#define KLEVEBRAND_SKYWIRE_FRAMEWORK_H

#include <SoftwareSerial.h>

#define SKYWIRE_RX_PIN 10
#define SKYWIRE_TX_PIN 11

class Skywire {
public:
    void start();
    void send();
    void stop();

private:
    static SoftwareSerial skywire;
};

#endif // KLEVEBRAND_SKYWIRE_FRAMEWORK_H
