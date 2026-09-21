#include "skywire-command-socket-configure.h"
#include "skywire_strstr_p.h"

SocketConfigureSkywireCommand::SocketConfigureSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#SCFG=1,1,300,90,600,50"), debug_mode, on_completed_function),
      _state(State::SendConfigure),
      _recovery_started_timestamp(0)
{
}

bool SocketConfigureSkywireCommand::socketSetupFailed() const
{
    return skywireContainsP(SkywireAtEngine::getRxBuffer(), PSTR("+CME ERROR: can not setup socket"));
}

bool SocketConfigureSkywireCommand::hasReceivedSocketCloseResponse() const
{
    char *const rx_buffer = SkywireAtEngine::getRxBuffer();

    return skywireContainsP(rx_buffer, PSTR("\r\nOK\r\n")) ||
           skywireContainsP(rx_buffer, PSTR("ERROR")) ||
           skywireContainsP(rx_buffer, PSTR("+CME ERROR"));
}

SkywireResponseResult_t SocketConfigureSkywireCommand::process()
{
    char *rx_buffer = SkywireAtEngine::getRxBuffer();
    const unsigned long now = millis();

    if (_at.completed())
    {
        return {true, rx_buffer};
    }

    _at.recordFirstProcessCall();

    switch (_state)
    {
    case State::SendConfigure:
        if (!_at.hasSent() &&
            now - _at.getFirstProcessCallTimestamp() > 200 &&
            _at.getFirstProcessCallTimestamp() != 0)
        {
            _at.resetRxBuffer();
            _at.writeCommandToModem();
            _at.setSent(true);
            _state = State::WaitConfigure;
        }

        return {false, rx_buffer};

    case State::WaitConfigure:
        _at.serialReadToRxBuffer();
        rx_buffer = SkywireAtEngine::getRxBuffer();

        if (socketSetupFailed())
        {
            if (SkywireAtEngine::debugMode())
            {
                Serial.println(F("Socket configure failed, closing socket before retry."));
            }

            _at.resetRxBuffer();
            _at.setSent(false);
            _recovery_started_timestamp = now;
            _state = State::SendClose;

            return {false, rx_buffer};
        }

        if (!_at.okReceived())
        {
            return {false, rx_buffer};
        }

        _at.notifyCompletedIfNeeded();
        _at.setCompleted(true);

        return {true, rx_buffer};

    case State::SendClose:
        if (!_at.hasSent())
        {
            _at.printToModem(F("AT#SH=1\r"));

            if (SkywireAtEngine::debugMode())
            {
                Serial.println(F("AT#SH=1"));
            }

            _at.setSent(true);
            _state = State::WaitClose;
        }

        return {false, rx_buffer};

    case State::WaitClose:
        _at.serialReadToRxBuffer();

        if (!hasReceivedSocketCloseResponse())
        {
            if (_recovery_started_timestamp != 0 && now - _recovery_started_timestamp > 1000)
            {
                _at.resetRxBuffer();
                _at.setSent(false);
                _recovery_started_timestamp = now;
                _state = State::SendClose;
            }

            return {false, rx_buffer};
        }

        _at.resetRxBuffer();
        _at.setSent(false);
        _recovery_started_timestamp = 0;
        _state = State::SendConfigure;

        return {false, rx_buffer};
    }

    return {false, rx_buffer};
}

void SocketConfigureSkywireCommand::reset()
{
    _at.reset();
    _state = State::SendConfigure;
    _recovery_started_timestamp = 0;
}
