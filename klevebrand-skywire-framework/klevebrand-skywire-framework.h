#ifndef KLEVEBRAND_SKYWIRE_FRAMEWORK_H
#define KLEVEBRAND_SKYWIRE_FRAMEWORK_H

#include <SoftwareSerial.h>

#define SKYWIRE_RX_PIN 2
#define SKYWIRE_TX_PIN 8
#define BASE_DELAY 200
#define BASE_WAIT_FOR_RESPONSE_DELAY 5000

class Skywire {
public:
    static void start();
    static void stop();
    static SoftwareSerial skywireSerialChannel;
private:
    static void configureHologramApn();
    static bool isHologramApnSuccessfullyConfigured();
    static bool isHologramApnSuccessfullyConfiguredResponseOk(String responseContent);
    static void sendAT();
    static void disableEcho();
    static String getIpAddress();
    static String getSignalStrengthAndQuality(); 
    static bool isConnectedToNetwork();
    static String getConnectedNetworkProviderName();
    static bool isConnectedToNetworkResponseOk(String responseContent);
    static void waitUntilConnectedToHomeNetwork();
    static void enableVerboseOutput();
    static void enablePacketDataProtocol();
    static bool waitForSkywireResponse(int millisecondsToWait, bool (*isResponseValid)(String));
    static bool responseOkSerialPrint(String responseContent);
};

#endif // KLEVEBRAND_SKYWIRE_FRAMEWORK_H
