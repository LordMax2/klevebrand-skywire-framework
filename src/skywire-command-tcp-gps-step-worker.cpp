#include "skywire-command-tcp-gps-step-worker.h"
#include "skywire_strstr_p.h"

DroneRequest_t SkywireTcpGpsStepWorker::_latest_drone_request_response = DroneRequest_t::empty();
GpsLocationInfo_t SkywireTcpGpsStepWorker::_latest_gps_response = GpsLocationInfo_t::empty();

SkywireTcpGpsStepWorker::SkywireTcpGpsStepWorker(
    HardwareSerial *skywire_serial,
    const char *host,
    const int port,
    const unsigned long timeout_milliseconds,
    const bool debug_mode)
    : _stepper(timeout_milliseconds),
      _gps_command(skywire_serial, debug_mode, setLatestGpsResponse),
      _socket_configure_command(skywire_serial, debug_mode, nullptr),
      _socket_connect_command(skywire_serial, host, port, debug_mode, nullptr),
      _request_command(skywire_serial, debug_mode, F("GetDroneRequest|1337"), true, setLatestTcpResponse),
      _state_message{0},
      _send_state_command(skywire_serial, debug_mode, _state_message, false, nullptr),
      _socket_close_command(skywire_serial, debug_mode, nullptr)
{
}

void SkywireTcpGpsStepWorker::reset()
{
    _stepper.reset(
        _gps_command,
        _socket_configure_command,
        _socket_connect_command,
        _request_command,
        _send_state_command,
        _socket_close_command);
}

void SkywireTcpGpsStepWorker::setPayloadToSend(const char *payload)
{
    strcpy_P(_state_message, PSTR("SetDroneState|1337|"));
    strncat(_state_message, payload != nullptr ? payload : "", sizeof(_state_message) - strlen(_state_message) - 1);
}

bool SkywireTcpGpsStepWorker::run()
{
    if (_stepper.isFinished())
    {
        reset();

        return true;
    }

    const SkywireStepResult result = _stepper.stepCurrent(
        _gps_command,
        _socket_configure_command,
        _socket_connect_command,
        _request_command,
        _send_state_command,
        _socket_close_command);

    if (result == SkywireStepResult::TimedOut)
    {
        reset();
        SkywireAtEngine::rebeginModem();
    }
    else if (result == SkywireStepResult::Finished)
    {
        reset();

        return true;
    }

    return false;
}

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
            const size_t token_length = strlen_P(tokens_to_remove[i]);
            if (token_length > 0 && strncmp_P(read_cursor, tokens_to_remove[i], token_length) == 0)
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

    char *line_break = skywireStrstrP(payload, PSTR("\r\n"));
    if (line_break != nullptr)
    {
        payload = line_break + 2;
    }

    char *ok_marker = skywireStrstrP(payload, PSTR("\r\nOK\r\n"));
    if (ok_marker != nullptr)
    {
        *ok_marker = '\0';
    }

    line_break = skywireStrstrP(payload, PSTR("\r\n"));
    if (line_break != nullptr)
    {
        payload = line_break + 2;
    }

    static const char kTokenSrecv[] PROGMEM = "#SRECV: 1,";
    static const char kTokenOk[] PROGMEM = "OK";
    static const char kTokenPrompt[] PROGMEM = "<<<";
    static const char kTokenError[] PROGMEM = "ERROR";
    static const char kTokenCme[] PROGMEM = "+CME";
    static const char kTokenCr[] PROGMEM = "\r";
    static const char kTokenLf[] PROGMEM = "\n";
    static const char *const tokens_to_remove[] = {
        kTokenSrecv,
        kTokenOk,
        kTokenPrompt,
        kTokenError,
        kTokenCme,
        kTokenCr,
        kTokenLf};

    compactResponse(payload, tokens_to_remove, sizeof(tokens_to_remove) / sizeof(tokens_to_remove[0]));

    _latest_drone_request_response = DroneRequest_t::parseFromCsvString(payload);
}

DroneRequest_t SkywireTcpGpsStepWorker::getLatestDroneRequest()
{
    return _latest_drone_request_response;
}

void SkywireTcpGpsStepWorker::setLatestGpsResponse(char *response)
{
    static const char kTokenOk[] PROGMEM = "OK";
    static const char kTokenGpsacp[] PROGMEM = "$GPSACP: ";
    static const char kTokenCr[] PROGMEM = "\r";
    static const char kTokenLf[] PROGMEM = "\n";
    static const char kTokenColon[] PROGMEM = ":";
    static const char kTokenSpace[] PROGMEM = " ";
    static const char kTokenWrong[] PROGMEM = "wrong";
    static const char kTokenState[] PROGMEM = "state";
    static const char *const tokens_to_remove[] = {
        kTokenOk,
        kTokenGpsacp,
        kTokenCr,
        kTokenLf,
        kTokenColon,
        kTokenSpace,
        kTokenWrong,
        kTokenState};

    compactResponse(response, tokens_to_remove, sizeof(tokens_to_remove) / sizeof(tokens_to_remove[0]));

    _latest_gps_response = GpsLocationInfo_t::parseFromGpsAcpString(response);
}

GpsLocationInfo_t SkywireTcpGpsStepWorker::getLatestGpsResponse()
{
    return _latest_gps_response;
}
