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
    void disableEcho();
    void sendAT();
    String getIpAddress();
    String getSignalStrengthAndQuality(); // Should not be of type string, should be something like tuple or class
    bool isConnectedToNetwork();
    String getConnectedNetworkProviderName();
};

#endif // KLEVEBRAND_SKYWIRE_FRAMEWORK_H
