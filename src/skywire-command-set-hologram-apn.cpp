#include "skywire-command-set-hologram-apn.h"
#include "skywire_flash_string.h"

SetApnHologramSkywireCommand::SetApnHologramSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT+CGDCONT=1,\"IPV4V6\",\"hologram\""), debug_mode, on_completed_function),
      _state(State::SendSet)
{
}

SkywireResponseResult_t SetApnHologramSkywireCommand::process()
{
    char *rx_buffer = SkywireAtEngine::getRxBuffer();

    if (_at.completed())
    {
        return {true, rx_buffer};
    }

    const unsigned long now = millis();

    switch (_state)
    {
    case State::SendSet:
        _at.recordFirstProcessCall();

        if (!_at.hasSent() &&
            millis() - _at.getFirstProcessCallTimestamp() > 200 &&
            _at.getFirstProcessCallTimestamp() != 0)
        {
            _at.resetRxBuffer();
            _at.writeCommandToModem();
            _at.setSent(true);
            _state = State::WaitSet;
        }

        break;

    case State::WaitSet:
        _at.serialReadToRxBuffer();

        if (_at.okReceived())
        {
            _state = State::SendQuery;
            _at.setSent(false);
        }

        break;

    case State::SendQuery:
        if (!_at.hasSent())
        {
            _at.resetRxBuffer();
            _at.printToModem(F("AT+CGDCONT?\r"));

            if (SkywireAtEngine::debugMode())
            {
                Serial.println(F("AT+CGDCONT?"));
            }

            _at.setSent(true);
        }

        _state = State::WaitQuery;

        break;

    case State::WaitQuery:
    {
        _at.serialReadToRxBuffer();
        rx_buffer = SkywireAtEngine::getRxBuffer();

        const bool has_hologram_ok = skywireContainsFlashString(rx_buffer, PSTR("hologram"));
        const bool already_active = skywireContainsFlashString(rx_buffer, PSTR("+CME ERROR: context already activated"));

        if (has_hologram_ok || already_active)
        {
            _state = State::Done;
            _at.setCompleted(true);
            _at.notifyCompletedIfNeeded();

            return {true, rx_buffer};
        }

        if (_at.okReceived())
        {
            reset();
        }
    }
    break;

    case State::Done:
        return {true, rx_buffer};
    }

    if (_at.hasSent() && now - _at.getLastSendTimestamp() >= 1000)
    {
        if (_state == State::WaitSet)
        {
            _at.writeCommandToModem();
        }
        else if (_state == State::WaitQuery)
        {
            _at.resetRxBuffer();
            _at.printToModem(F("AT+CGDCONT?\r"));

            if (SkywireAtEngine::debugMode())
            {
                Serial.println(F("AT+CGDCONT?"));
            }
        }

        _at.setSent(true);
    }

    return {false, rx_buffer};
}

void SetApnHologramSkywireCommand::reset()
{
    _at.reset();
    _state = State::SendSet;
}
