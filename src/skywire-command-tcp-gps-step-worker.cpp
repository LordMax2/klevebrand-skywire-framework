#include "skywire-command-tcp-gps-step-worker.h"

DroneRequest_t SkywireTcpGpsStepWorker::_latest_drone_request_response = DroneRequest_t::empty();
GpsLocationInfo_t SkywireTcpGpsStepWorker::_latest_gps_response = GpsLocationInfo_t::empty();

namespace
{
void compactResponse(char *buffer, const char *const *tokens_to_remove, size_t token_count)
{
    char *write_cursor = buffer;
    const char *read_cursor = buffer;

    while (*read_cursor != '\0')
    {
        bool removed = false;
        for (size_t i = 0; i < token_count; ++i)
        {
            const size_t token_length = strlen(tokens_to_remove[i]);
            if (token_length > 0 && strncmp(read_cursor, tokens_to_remove[i], token_length) == 0)
            {
                read_cursor += token_length;
                removed = true;
                break;
            }
        }

        if (!removed)
        {
            *write_cursor++ = *read_cursor++;
        }
    }

    *write_cursor = '\0';
}
}

void SkywireTcpGpsStepWorker::setLatestTcpResponse(char *response)
{
    char *payload = response;

    char *line_break = strstr(payload, "\r\n");
    if (line_break != nullptr)
    {
        payload = line_break + 2;
    }

    char *ok_marker = strstr(payload, "\r\nOK\r\n");
    if (ok_marker != nullptr)
    {
        *ok_marker = '\0';
    }

    line_break = strstr(payload, "\r\n");
    if (line_break != nullptr)
    {
        payload = line_break + 2;
    }

    static const char *tokens_to_remove[] = {"#SRECV: 1,", "OK", "<<<", "ERROR", "+CME", "\r", "\n"};
    compactResponse(payload, tokens_to_remove, sizeof(tokens_to_remove) / sizeof(tokens_to_remove[0]));

    _latest_drone_request_response = DroneRequest_t::parseFromCsvString(payload);
}

DroneRequest_t SkywireTcpGpsStepWorker::getLatestDroneRequest()
{
    return _latest_drone_request_response;
}

void SkywireTcpGpsStepWorker::setLatestGpsResponse(char *response)
{
    static const char *tokens_to_remove[] = {"OK", "$GPSACP: ", "\r", "\n", ":", " ", "wrong", "state"};
    compactResponse(response, tokens_to_remove, sizeof(tokens_to_remove) / sizeof(tokens_to_remove[0]));

    _latest_gps_response = GpsLocationInfo_t::parseFromGpsAcpString(response);
}

GpsLocationInfo_t SkywireTcpGpsStepWorker::getLatestGpsResponse() {
    return _latest_gps_response;
}
