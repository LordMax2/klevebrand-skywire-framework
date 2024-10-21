#ifndef KLEVEBRAND_SKYWIRE_FRAMEWORK_H
#define KLEVEBRAND_SKYWIRE_FRAMEWORK_H

#include <SoftwareSerial.h>

#define SKYWIRE_RX_PIN 2
#define SKYWIRE_TX_PIN 8

class Skywire {
public:
    void start();
    void stop();
    static SoftwareSerial skywireSerialChannel;
private:
    bool isHologramApnSuccessfullyConfigured();
    void configureHologramApn();
};

#endif // KLEVEBRAND_SKYWIRE_FRAMEWORK_H
