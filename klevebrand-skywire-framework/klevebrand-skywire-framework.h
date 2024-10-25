#ifndef KLEVEBRAND_SKYWIRE_FRAMEWORK_H
#define KLEVEBRAND_SKYWIRE_FRAMEWORK_H

#include <SoftwareSerial.h>

#define SKYWIRE_RX_PIN 2
#define SKYWIRE_TX_PIN 8
#define BASE_DELAY 200

class Skywire {
public:
    void start();
    void stop();
    static SoftwareSerial skywireSerialChannel;
    void getAssignment();
private:
    bool isHologramApnSuccessfullyConfigured();
    void configureHologramApn();
    void disableEcho();
    void sendAT();
    String getIpAddress();
    String getSignalStrengthAndQuality(); // Should not be of type string, should be something like tuple or class
    bool isConnectedToNetwork();
    String getConnectedNetworkProviderName();
    void waitUntilConnectedToHomeNetwork();
    void enableVerboseOutput();
    void enablePacketDataProtocol();
};

#endif // KLEVEBRAND_SKYWIRE_FRAMEWORK_H
