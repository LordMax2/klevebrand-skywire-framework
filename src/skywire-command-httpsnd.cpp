#include "skywire-command-httpsnd.h"

#if SKYWIRE_ENABLE_HTTP

#include "skywire_strstr_p.h"

HttpSndSkywireCommand::HttpSndSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const char path[HTTP_SND_PATH_SIZE],
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#HTTPSND=0,0,"), debug_mode, on_completed_function),
      _payload_sent(false),
      _ok_received(false)
{
    strncpy(_path, path != nullptr ? path : "", sizeof(_path) - 1);
    _path[sizeof(_path) - 1] = '\0';
    _payload[0] = '\0';
}

bool HttpSndSkywireCommand::arrowsReceived() const
{
    return skywireContainsP(SkywireAtEngine::getRxBuffer(), PSTR(">"));
}

void HttpSndSkywireCommand::setPayload(const char *payload_to_send)
{
    strncpy(_payload, payload_to_send != nullptr ? payload_to_send : "", sizeof(_payload) - 1);
    _payload[sizeof(_payload) - 1] = '\0';
}

void HttpSndSkywireCommand::reset()
{
    _at.reset();
    _payload_sent = false;
    _ok_received = false;
}

bool HttpSndSkywireCommand::okReceived() const
{
    return _ok_received || skywireContainsP(SkywireAtEngine::getRxBuffer(), PSTR("\r\nOK\r\n"));
}

SkywireResponseResult_t HttpSndSkywireCommand::process()
{
    char *rx_buffer = SkywireAtEngine::getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    const unsigned long now = millis();

    _at.setFirstProcessCall();

    if (!_at.isSent())
    {
        if (now - _at.getFirstProcessCallTimestamp() > 200 && _at.getFirstProcessCallTimestamp() != 0)
        {
            _at.resetRxBuffer();

            if (SkywireAtEngine::debugMode())
            {
                Serial.print(_at.command());
                Serial.print(_path);
                Serial.print(F(","));
                Serial.print(strlen(_payload));
                Serial.print(F("\r"));
                Serial.println();
            }

            _at.printToModem(_at.command());
            _at.printToModem(_path);
            _at.printToModem(F(","));
            _at.printToModem(static_cast<int>(strlen(_payload)));
            _at.printToModem('\r');
            _at.setSent(true);
        }

        return {false, rx_buffer};
    }

    _at.serialReadToRxBuffer();
    rx_buffer = SkywireAtEngine::getRxBuffer();

    if (okReceived() && _at.isSent())
    {
        if (!_ok_received)
        {
            _at.resetRxBuffer();
            if (SkywireAtEngine::debugMode())
            {
                Serial.println(F("CLEAR"));
            }

            _ok_received = true;
        }
    }
    else
    {
        return {false, rx_buffer};
    }

    if (_at.isSent() && arrowsReceived() && !_payload_sent)
    {
        if (SkywireAtEngine::debugMode())
        {
            Serial.print(F("HTTPSND Sending payload: "));
            Serial.println(_payload);
        }

        _at.printToModem(_payload);
        _at.writeToModem(0x1A);
        _payload_sent = true;

        if (SkywireAtEngine::debugMode())
        {
            Serial.println(F("HTTPSND payload sent, waiting for final response"));
        }
    }

    const bool is_complete = completed();
    if (is_complete)
    {
        _at.notifyCompletedIfNeeded();
        _at.setCompleted(true);
    }

    return {is_complete, rx_buffer};
}

bool HttpSndSkywireCommand::completed() const
{
    return _at.isCompletedFlag() || (_payload[0] == '\0') || (_at.isSent() && _payload_sent && okReceived());
}

#endif
