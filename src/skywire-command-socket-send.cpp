#include "skywire-command-socket-send.h"
#include "skywire_strstr_p.h"

SocketSendSkywireCommand::SocketSendSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const __FlashStringHelper *flash_message,
    const bool read_response,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#SSEND=1"), debug_mode, on_completed_function),
      _flash_message(flash_message),
      _message(nullptr),
      _last_read_timestamp(0),
      _send_flags(read_response ? kFlagReadResponse : 0)
{
}

SocketSendSkywireCommand::SocketSendSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    char *message,
    const bool read_response,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#SSEND=1"), debug_mode, on_completed_function),
      _flash_message(nullptr),
      _message(message),
      _last_read_timestamp(0),
      _send_flags(read_response ? kFlagReadResponse : 0)
{
}

bool SocketSendSkywireCommand::flag(const uint8_t mask) const
{
    return (_send_flags & mask) != 0;
}

void SocketSendSkywireCommand::setFlag(const uint8_t mask, const bool value)
{
    if (value)
    {
        _send_flags = static_cast<uint8_t>(_send_flags | mask);
    }
    else
    {
        _send_flags = static_cast<uint8_t>(_send_flags & static_cast<uint8_t>(~mask));
    }
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
    return skywireContainsP(SkywireAtEngine::getRxBuffer(), PSTR(">"));
}

bool SocketSendSkywireCommand::responseReceived() const
{
    char *const rx_buffer = SkywireAtEngine::getRxBuffer();

    return skywireContainsP(rx_buffer, PSTR("#SRECV:")) &&
           skywireContainsP(rx_buffer, PSTR("\r\nOK\r\n"));
}

bool SocketSendSkywireCommand::responseRetrySuggested() const
{
    char *const rx_buffer = SkywireAtEngine::getRxBuffer();

    return skywireContainsP(rx_buffer, PSTR("SRING")) ||
           skywireContainsP(rx_buffer, PSTR("+CME ERROR: operation not supported"));
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
    _last_read_timestamp = millis();
    setFlag(kFlagResponseRequested, true);

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

    _at.setFirstProcessCall();
    const unsigned long now = millis();

    if (!_at.isSent())
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

    if (!flag(kFlagPayloadSent))
    {
        if (!promptReceived())
        {
            return {false, rx_buffer};
        }

        writePayloadToModem();
        setFlag(kFlagPayloadSent, true);

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

    if (!flag(kFlagReadResponse))
    {
        if (!_at.okReceived())
        {
            return {false, rx_buffer};
        }
    }
    else
    {
        if (!flag(kFlagResponseRequested) && _at.okReceived())
        {
            _at.resetRxBuffer();
            readSocketResponse();

            return {false, rx_buffer};
        }

        if (flag(kFlagResponseRequested) &&
            !flag(kFlagResponseReceived) &&
            now - _last_read_timestamp > 200 &&
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

        setFlag(kFlagResponseReceived, true);
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

    if (flag(kFlagReadResponse))
    {
        return _at.isCompletedFlag() || flag(kFlagResponseReceived);
    }

    return _at.isCompletedFlag() || (flag(kFlagPayloadSent) && _at.okReceived());
}

void SocketSendSkywireCommand::reset()
{
    const bool read_response = flag(kFlagReadResponse);

    _at.reset();
    _last_read_timestamp = 0;
    _send_flags = read_response ? kFlagReadResponse : 0;
}
