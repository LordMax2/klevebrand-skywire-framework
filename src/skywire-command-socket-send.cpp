#include "skywire-command-socket-send.h"
#include "skywire_flash_string.h"

SocketSendSkywireCommand::SocketSendSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const __FlashStringHelper *flash_message,
    const bool should_read_response,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#SSEND=1"), debug_mode, on_completed_function),
      _flash_message(flash_message),
      _message(nullptr),
      _last_response_request_timestamp(0),
      _should_read_response(should_read_response),
      _has_sent_payload(false),
      _has_requested_response(false)
{
}

SocketSendSkywireCommand::SocketSendSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    char *message,
    const bool should_read_response,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#SSEND=1"), debug_mode, on_completed_function),
      _flash_message(nullptr),
      _message(message),
      _last_response_request_timestamp(0),
      _should_read_response(should_read_response),
      _has_sent_payload(false),
      _has_requested_response(false)
{
}

bool SocketSendSkywireCommand::hasPayload() const
{
    if (_flash_message != nullptr)
    {
        return true;
    }

    return _message != nullptr && _message[0] != '\0';
}

bool SocketSendSkywireCommand::promptReceived() const
{
    return skywireContainsFlashString(SkywireAtEngine::getRxBuffer(), PSTR(">"));
}

bool SocketSendSkywireCommand::responseReceived() const
{
    char *const rx_buffer = SkywireAtEngine::getRxBuffer();

    return skywireContainsFlashString(rx_buffer, PSTR("#SRECV:")) &&
           skywireContainsFlashString(rx_buffer, PSTR("\r\nOK\r\n"));
}

bool SocketSendSkywireCommand::responseRetrySuggested() const
{
    char *const rx_buffer = SkywireAtEngine::getRxBuffer();

    return skywireContainsFlashString(rx_buffer, PSTR("SRING")) ||
           skywireContainsFlashString(rx_buffer, PSTR("+CME ERROR: operation not supported"));
}

void SocketSendSkywireCommand::writePayloadToModem()
{
    if (_flash_message != nullptr)
    {
        _at.printToModem(_flash_message);
    }
    else if (_message != nullptr)
    {
        _at.printToModem(_message);
    }

    _at.writeToModem(0x1A);
}

void SocketSendSkywireCommand::readSocketResponse()
{
    _at.printToModem(F("AT#SRECV=1,"));
    _at.printToModem(static_cast<int>(SKYWIRE_SRECV_SIZE));
    _at.printToModem('\r');
    _at.setSent(true);
    _last_response_request_timestamp = millis();
    _has_requested_response = true;

    if (SkywireAtEngine::debugMode())
    {
        Serial.print(F("AT#SRECV=1,"));
        Serial.println(static_cast<int>(SKYWIRE_SRECV_SIZE));
    }
}

SkywireResponseResult_t SocketSendSkywireCommand::process()
{
    char *rx_buffer = SkywireAtEngine::getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    _at.recordFirstProcessCall();
    const unsigned long now = millis();

    if (!_at.hasSent())
    {
        if (now - _at.getFirstProcessCallTimestamp() > 200 && _at.getFirstProcessCallTimestamp() != 0)
        {
            _at.resetRxBuffer();
            _at.writeCommandToModem();
            _at.setSent(true);
        }

        return {false, rx_buffer};
    }

    _at.serialReadToRxBuffer();
    rx_buffer = SkywireAtEngine::getRxBuffer();

    if (!_has_sent_payload)
    {
        if (!promptReceived())
        {
            return {false, rx_buffer};
        }

        writePayloadToModem();
        _has_sent_payload = true;

        if (SkywireAtEngine::debugMode())
        {
            Serial.print(F("Socket payload: "));
            if (_flash_message != nullptr)
            {
                Serial.println(_flash_message);
            }
            else
            {
                Serial.println(_message != nullptr ? _message : "");
            }
        }

        _at.resetRxBuffer();

        return {false, rx_buffer};
    }

    if (!_should_read_response)
    {
        if (!_at.okReceived())
        {
            return {false, rx_buffer};
        }
    }
    else
    {
        if (!_has_requested_response && _at.okReceived())
        {
            _at.resetRxBuffer();
            readSocketResponse();

            return {false, rx_buffer};
        }

        if (_has_requested_response &&
            now - _last_response_request_timestamp > 200 &&
            (responseRetrySuggested() || !_at.modemAvailable()))
        {
            _at.resetRxBuffer();
            readSocketResponse();

            return {false, rx_buffer};
        }

        if (!responseReceived())
        {
            return {false, rx_buffer};
        }
    }

    _at.notifyCompletedIfNeeded();
    _at.setCompleted(true);

    return {true, rx_buffer};
}

bool SocketSendSkywireCommand::completed() const
{
    if (!hasPayload())
    {
        return true;
    }

    if (_should_read_response)
    {
        return _at.hasMarkedCompleted();
    }

    return _at.hasMarkedCompleted() || (_has_sent_payload && _at.okReceived());
}

void SocketSendSkywireCommand::reset()
{
    _at.reset();
    _last_response_request_timestamp = 0;
    _has_sent_payload = false;
    _has_requested_response = false;
}
