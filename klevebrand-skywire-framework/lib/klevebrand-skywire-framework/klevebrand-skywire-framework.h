#ifndef KLEVEBRAND_SKYWIRE_FRAMEWORK_H
#define KLEVEBRAND_SKYWIRE_FRAMEWORK_H

#include <SoftwareSerial.h>

#define SKYWIRE_RX_PIN 2
#define SKYWIRE_TX_PIN 8

#define BASE_DELAY 200
#define BASE_WAIT_FOR_RESPONSE_DELAY 5000

#define DEBUG true

class Skywire
{
public:
    /*
     * We expose the SoftwareSerial channel so that this class can be wrapped and extended
     * for more specialized use-cases.
     * Use serial communication with this object to communicate with the Skywire modem.
     */
    static SoftwareSerial skywireSerialChannel;

    /*
     * Starts the Skywire modem and configures it currently with 'hologram' as APN.
     * The start method will try to configure and start the mode, and also verify the
     * configuration by verifying the state of the Skywire modem.
     */
    static void start();

    /*
     * Shuts down the Skywire modem.
     */
    static void stop();

    static int available();

    static String readString();

    static size_t print(String payload);

    /*
     * Tries to get the current ip address of the modem.
     * If the modem is not connected to a network, and fails to start, this will just return
     * an empty string.
     */
    static String getIpAddress();

    /*
     * Tries to get the current signal strength.
     * If the modem is not connected to a network, and fails to start, this will just return
     * an empty string.
     */
    static String getSignalStrengthAndQuality();

    /*
     * Tries to get the name of the connected APN 4g tower owner.
     * If the modem is not connected to a network, and fails to start, this will just return
     * an empty string.
     */
    static String getConnectedNetworkProviderName();

    /*
     * This method will wait for serial responses from the Skywire modem, and use the isResponseValid
     * parameter function to validate each line comming from the modem.
     * If the isResponseValid method returns true when reciving data from the modem, this entire method
     * waitForSkywireResponse will also return true immediatly.
     * The maximum wait period for a valid message is the input parameter millisecondsToWait.
     * Otherwise it will return false.
     */
    static bool waitForSkywireResponse(int millisecondsToWait, bool (*isResponseValid)(String));

    /*
     * Enables verbose output on the Skywire modem.
     * Can be helpful when debugging and during development.
     */
    static void enableVerboseOutput();

    /*
     * Tries to open a TCP socket to the endpoint supplied in the parameters.
     */
    static int openTcpSocketConnection(String ipAddress, int port);

    /*
     * Tries to send a message in the open TCP socket.
     * If the socket is down, it will try to re-open the socket and then try to send the message.
     */
    static bool sendMessageInTcpSocketConnection(String message, int socketDialConnectionId);

    /*
     * Tries to close the TCP channel.
     */
    static bool closeTcpSocketConnection(int socketDialConnectionId);

private:
    /*
     * Configures 'hologram' as APN, will verify the state of the modem and retry the
     * configuration until it is successful.
     */
    static void configureHologramApn();

    /*
     * Validates the configuration and checks if hologram is configured as an APN.
     */
    static bool isHologramApnSuccessfullyConfigured();

    /*
     * Validates the response of isHologramApnSuccessfullyConfigured to make sure
     * that 'hologram' exists in any of the response messages from the Skywire modem.
     */
    static bool isHologramApnSuccessfullyConfiguredResponseOk(String responseContent);

    /*
     * Just sends a regular AT command to the Skywire modem, the modem should respond with
     * an 'OK' here.
     * Can be used to do a 'connection check' or 'alive check'.
     */
    static void sendAt();

    static void send(String payload);

    /*
     * Turns off echo on the Skywire modem.
     */
    static void disableEcho();

    /*
     * Validates that the current state of the modem is connected to a network.
     */
    static bool isConnectedToNetwork();

    /*
     * Validates the response of isConnectedToNetwork to make sure that we are connected to
     * a network.
     */
    static bool isConnectedToNetworkResponseOk(String responseContent);
    static void waitUntilConnectedToHomeNetwork();
    static void enablePacketDataProtocol();
    static bool responseOkSerialPrint(String responseContent);
    static bool isOpenTcpSocketConnectionResponseOk(String responseContent);
};

#endif // KLEVEBRAND_SKYWIRE_FRAMEWORK_H
