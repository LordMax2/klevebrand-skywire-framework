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
        bool debug_mode) : SkywireCommandWorker(skywire_serial, debug_mode, timeout_milliseconds, TCP_GPS_STEP_COUNT)
    {
        this->steps[0] = new GpsAcpSkywireCommand(skywire, debug_mode, setLatestGpsResponse);
        this->steps[1] = new SocketConfigureSkywireCommand(skywire, debug_mode, nullptr);
        this->steps[2] = new SocketConnectSkywireCommand(skywire, host, port, debug_mode, nullptr);
        this->steps[3] = new SocketSendSkywireCommand(skywire, debug_mode, "GetDroneRequest|1337", true, setLatestTcpResponse);
        this->steps[4] = new SocketSendSkywireCommand(skywire, debug_mode, "", false, nullptr);
        this->steps[5] = new SocketCloseSkywireCommand(skywire, debug_mode, nullptr);

        reset();
    }

    void setPayloadToSend(const char *payload)
    {
        String message = "SetDroneState|1337|";
        message += payload;
        static_cast<SocketSendSkywireCommand *>(steps[4])->setMessage(message.c_str());
    }

    GpsLocationInfo_t getLatestGpsResponse();
    DroneRequest_t getLatestDroneRequest();
    static void setLatestTcpResponse(char response[512]);
    static void setLatestGpsResponse(char response[512]);

private:
    static GpsLocationInfo_t _latest_gps_response;
    static DroneRequest_t _latest_drone_request_response;
};

#endif
