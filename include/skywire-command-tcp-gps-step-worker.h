#pragma once

#ifndef SKYWIRE_COMMAND_TCP_GPS_STEP_WORKER_H
#define SKYWIRE_COMMAND_TCP_GPS_STEP_WORKER_H

#include "Arduino.h"
#include "skywire_config.h"
#include "skywire_stepper.h"
#include "skywire_at_engine.h"
#include "concept_skywire_worker.h"
#include "skywire-command-gpsacp.h"
#include "skywire-command-socket-configure.h"
#include "skywire-command-socket-connect.h"
#include "skywire-command-socket-send.h"
#include "skywire-command-socket-close.h"
#include "gps_location_info.h"
#include "drone_request.h"

#define TCP_GPS_STEP_COUNT 6

class SkywireTcpGpsStepWorker
{
public:
    SkywireTcpGpsStepWorker(
        HardwareSerial *skywire_serial,
        const char *host,
        int port,
        unsigned long timeout_milliseconds,
        bool debug_mode);

    bool run();
    void reset();
    void setPayloadToSend(const char *payload);

    GpsLocationInfo_t getLatestGpsResponse();
    DroneRequest_t getLatestDroneRequest();
    static void setLatestTcpResponse(char *response);
    static void setLatestGpsResponse(char *response);

private:
    static GpsLocationInfo_t _latest_gps_response;
    static DroneRequest_t _latest_drone_request_response;

    SkywireStepper<TCP_GPS_STEP_COUNT> _stepper;
    GpsAcpSkywireCommand _gps_command;
    SocketConfigureSkywireCommand _socket_configure_command;
    SocketConnectSkywireCommand _socket_connect_command;
    SocketSendSkywireCommand _request_command;
    char _state_message[SOCKET_SEND_MESSAGE_SIZE];
    SocketSendSkywireCommand _send_state_command;
    SocketCloseSkywireCommand _socket_close_command;
};

static_assert(SkywireWorkerConcept<SkywireTcpGpsStepWorker>, "SkywireTcpGpsStepWorker doesnt implement the concept");

#endif
