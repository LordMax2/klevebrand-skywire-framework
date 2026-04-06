#include "skywire-tcp-gps-step-worker.h"

DroneRequest_t SkywireTcpGpsStepWorker::_latest_drone_request_response = DroneRequest_t::empty();
GpsLocationInfo_t SkywireTcpGpsStepWorker::_latest_gps_response = GpsLocationInfo_t::empty();

void SkywireTcpGpsStepWorker::setLatestTcpResponse(char response[512])
{
    String response_copy = response;

    const int response_start = response_copy.indexOf("\r\n");
    if (response_start >= 0)
    {
        response_copy = response_copy.substring(response_start + 2);
    }

    const int response_end = response_copy.indexOf("\r\nOK\r\n");
    if (response_end >= 0)
    {
        response_copy = response_copy.substring(0, response_end);
    }

    const int data_start = response_copy.indexOf("\r\n");
    if (data_start >= 0)
    {
        response_copy = response_copy.substring(data_start + 2);
    }

    response_copy.replace("#SRECV: 1,", "");
    response_copy.replace("OK", "");
    response_copy.replace("<<<", "");
    response_copy.replace("ERROR", "");
    response_copy.replace("+CME", "");
    response_copy.replace("\r", "");
    response_copy.replace("\n", "");

    _latest_drone_request_response = DroneRequest_t::parseFromCsvString(response_copy);
}

DroneRequest_t SkywireTcpGpsStepWorker::getLatestDroneRequest()
{
    return _latest_drone_request_response;
}

void SkywireTcpGpsStepWorker::setLatestGpsResponse(char response[512])
{
    String response_copy = response;

    response_copy.replace("OK", "");
    response_copy.replace("$GPSACP: ", "");
    response_copy.replace("\r", "");
    response_copy.replace("\n", "");
    response_copy.replace(":", "");
    response_copy.replace(" ", "");
    response_copy.replace("wrong", "");
    response_copy.replace("state", "");

    _latest_gps_response = GpsLocationInfo_t::parseFromGpsAcpString(response_copy);
}

GpsLocationInfo_t SkywireTcpGpsStepWorker::getLatestGpsResponse() {
    return _latest_gps_response;
}
