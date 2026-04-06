#ifndef KLEVEBRAND_SKYWIRE_TCP_GPS_STEPPER_CLIENT_H
#define KLEVEBRAND_SKYWIRE_TCP_GPS_STEPPER_CLIENT_H

#include "Arduino.h"
#include "skywire-command-worker.h"
#include "skywire-command.h"
#include "skywire-command-gpsacp.h"
#include "skywire-command-socket-configure.h"
#include "skywire-command-socket-connect.h"
#include "skywire-command-socket-send.h"
#include "skywire-command-socket-close.h"
#include "gps_location_info.h"
#include "drone_request.h"

#define TCP_GPS_STEP_COUNT 6

class SkywireTcpGpsStepWorker : public SkywireCommandWorker
{
public:
    SkywireTcpGpsStepWorker(
        HardwareSerial *skywire_serial,
        const char *host,
        int port,
        unsigned long timeout_milliseconds,
        bool debug_mode) : SkywireCommandWorker(skywire_serial, debug_mode, timeout_milliseconds, TCP_GPS_STEP_COUNT),
                          gps_command(skywire_serial, debug_mode, setLatestGpsResponse),
                          socket_configure_command(skywire_serial, debug_mode, nullptr),
                          socket_connect_command(skywire_serial, host, port, debug_mode, nullptr),
                          request_command(skywire_serial, debug_mode, "GetDroneRequest|1337", true, setLatestTcpResponse),
                          send_state_command(skywire_serial, debug_mode, "", false, nullptr),
                          socket_close_command(skywire_serial, debug_mode, nullptr)
    {
        this->steps[0] = &gps_command;
        this->steps[1] = &socket_configure_command;
        this->steps[2] = &socket_connect_command;
        this->steps[3] = &request_command;
        this->steps[4] = &send_state_command;
        this->steps[5] = &socket_close_command;

        reset();
    }

    void setPayloadToSend(const char *payload)
    {
        char message[SOCKET_SEND_MESSAGE_SIZE];
        strncpy(message, "SetDroneState|1337|", sizeof(message) - 1);
        message[sizeof(message) - 1] = '\0';
        strncat(message, payload != nullptr ? payload : "", sizeof(message) - strlen(message) - 1);
        send_state_command.setMessage(message);
    }

    GpsLocationInfo_t getLatestGpsResponse();
    DroneRequest_t getLatestDroneRequest();
    static void setLatestTcpResponse(char *response);
    static void setLatestGpsResponse(char *response);

private:
    static GpsLocationInfo_t _latest_gps_response;
    static DroneRequest_t _latest_drone_request_response;
    GpsAcpSkywireCommand gps_command;
    SocketConfigureSkywireCommand socket_configure_command;
    SocketConnectSkywireCommand socket_connect_command;
    SocketSendSkywireCommand request_command;
    SocketSendSkywireCommand send_state_command;
    SocketCloseSkywireCommand socket_close_command;
};

#endif
