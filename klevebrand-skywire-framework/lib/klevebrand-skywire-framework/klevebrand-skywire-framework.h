#ifndef KLEVEBRAND_SKYWIRE_FRAMEWORK_H
#define KLEVEBRAND_SKYWIRE_FRAMEWORK_H

#define BASE_DELAY 200
#define BASE_WAIT_FOR_RESPONSE_DELAY 5000

#define DEBUG true

#include "Arduino.h"

struct SkywireResponseResult_t
{
    SkywireResponseResult_t(bool is_success, String response_content)
    {
        this->is_success = is_success;
        this->response_content = response_content;
    }
    bool is_success;
    String response_content;
};

class Skywire
{
public:
    Skywire(HardwareSerial& skywire_serial) : _skywire_serial(skywire_serial) {}

    /*
     * Starts the Skywire modem and configures it currently with 'hologram' as APN.
     * The start method will try to configure and start the mode, and also verify the
     * configuration by verifying the state of the Skywire modem.
     */
    void start();

    /*
     * Shuts down the Skywire modem.
     */
    void reboot();

    int available();

    String readString();

    size_t print(String payload);

    /*
     * Tries to get the current ip address of the modem.
     * If the modem is not connected to a network, and fails to start, this will just return
     * an empty string.
     */
    String getIpAddress();

    /*
     * Tries to get the current signal strength.
     * If the modem is not connected to a network, and fails to start, this will just return
     * an empty string.
     */
    String getSignalStrengthAndQuality();

    /*
     * Tries to get the name of the connected APN 4g tower owner.
     * If the modem is not connected to a network, and fails to start, this will just return
     * an empty string.
     */
    String getConnectedNetworkProviderName();

    /*
     * This method will wait for serial responses from the Skywire modem, and use the isResponseValid
     * parameter function to validate each line comming from the modem.
     * If the isResponseValid method returns true when reciving data from the modem, this entire method
     * waitForSkywireResponse will also return true immediatly.
     * The maximum wait period for a valid message is the input parameter millisecondsToWait.
     * Otherwise it will return false.
     */
    SkywireResponseResult_t waitForSkywireResponse(int millisecondsToWait);

    /*
     * Enables verbose output on the Skywire modem.
     * Can be helpful when debugging and during development.
     */
    bool enableVerboseOutput();

    /*
     * Tries to open a TCP socket to the endpoint supplied in the parameters.
     */
    int openTcpSocketConnection(String ipAddress, int port);

    /*
     * Tries to send a message in the open TCP socket.
     * If the socket is down, it will try to re-open the socket and then try to send the message.
     */
    bool sendMessageInTcpSocketConnection(String message, int socketDialConnectionId);

    /*
     * Tries to close the TCP channel.
     */
    bool closeTcpSocketConnection(int socketDialConnectionId);

    /*
     * Tries to close all the TCP channels.
     */
    bool closeAllTcpSocketConnection();

private:
    HardwareSerial& _skywire_serial;

    /*
     * Configures 'hologram' as APN, will verify the state of the modem and retry the
     * configuration until it is successful.
     */
    bool configureHologramApn();

    /*
     * Validates the configuration and checks if hologram is configured as an APN.
     */
    bool isHologramApnSuccessfullyConfigured();

    /*
     * Just sends a regular AT command to the Skywire modem, the modem should respond with
     * an 'OK' here.
     * Can be used to do a 'connection check' or 'alive check'.
     */
    bool sendAt();

    void send(String payload);

    /*
     * Turns off echo on the Skywire modem.
     */
    bool disableEcho();

    /*
     * Validates that the current state of the modem is connected to a network.
     */
    bool isConnectedToNetwork();

    /*
     * Validates the response of isConnectedToNetwork to make sure that we are connected to
     * a network.
     */
    void waitUntilConnectedToHomeNetwork();
    bool enablePacketDataProtocol();
    bool disablePacketDataProtocol();
};

#endif // KLEVEBRAND_SKYWIRE_FRAMEWORK_H
