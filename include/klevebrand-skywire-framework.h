#ifndef KLEVEBRAND_SKYWIRE_FRAMEWORK_H
#define KLEVEBRAND_SKYWIRE_FRAMEWORK_H

#define BASE_DELAY 200
#define BASE_WAIT_FOR_RESPONSE_DELAY 5000

#include "Arduino.h"
#include "klevebrand-skywire-response-result.h"

class Skywire
{
public:
    Skywire(HardwareSerial& skywire_serial, bool debug_mode) : _skywire_serial(skywire_serial), _debug_mode(debug_mode) {}

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

    int read();

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
    SkywireResponseResult_t waitForSkywireResponse(unsigned long millisecondsToWait);

    /*
     * Enables verbose output on the Skywire modem.
     * Can be helpful when debugging and during development.
     */
    bool setVerboseOutputLevel(int level);

    /*
     * Tries to open a TCP socket to the endpoint supplied in the parameters.
     * Returns the socket dial connection id. 
     */
    int openTcpSocketConnection(String ipAddress, int port);

    /*
     * Tries to send a message in the open TCP socket.
     * If the socket is down, it will try to re-open the socket and then try to send the message.
     */
    bool sendMessageInTcpSocketConnection(String message, int socket_dial_connection_id);

    /*
     * Tries to close the TCP channel.
     */
    bool closeTcpSocketConnection(int socket_dial_connection_id);

    /*
     * Tries to close all the TCP channels.
     */
    bool closeAllTcpSocketConnection();

    /*
     * Configures the http parameters for the Skywire modem.
     * For now, only the url and port is configurable, but here we can set headers etc. too.
     */
    bool httpConfigureParameters(String url, int port);

    /*
     * Sends an HTTP request to the configured URL and port.
     */
    bool httpSendRequest(String path);

    /*
     * Waits for an HTTP response ring from the Skywire modem.
     * Which means that we have received an HTTP response from the server.
     */
    bool httpWaitForHttpRing(unsigned long timeout_milliseconds);

    /*
     * Reads the HTTP response from the Skywire modem.
     */
    SkywireResponseResult_t httpReadResponse();

    /*
     * Enable GPS and power up the appropriate things on the Skywire modem to make GPS work.
     */
    bool enableGps();

    /*
     * Sends GPSACP command to get GPS data from the Skywire modem.
     */
    SkywireResponseResult_t getGpsData();

    /*
     * If this is enabled, the serial output will get spammed with GPS data.
     */
    bool setGpsDataStreamingMode(bool enable_streaming);

private:
    /*
     * Reference to the serial instance used to communicate with the Skywire modem.
     */
    HardwareSerial& _skywire_serial;

    /*
     * If debug mode is enabled, the Skywire class will print debug information to Serial.
     */
    bool _debug_mode = false;

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
    void waitUntilConnectedToNetwork();
    bool enablePacketDataProtocol();
    bool disablePacketDataProtocol();
};

#endif // KLEVEBRAND_SKYWIRE_FRAMEWORK_H
