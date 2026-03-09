#include "skywire-command-set-hologram-apn.h"

SetApnHologramSkywireCommand::SetApnHologramSkywireCommand(HardwareSerial *skywire, bool debug_mode, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "AT+CGDCONT=1,\"IPV4V6\",\"hologram\"", debug_mode, on_completed_function),
      state(State::SEND_SET)
{
}

SkywireResponseResult_t SetApnHologramSkywireCommand::process()
{
    const unsigned long now = millis();

    switch (state)
    {
    case State::SEND_SET:
        if (_first_process_call)
        {
            _first_process_call = false;
            _first_process_call_timestamp = millis();
        }

        if (!_sent && millis() - _first_process_call_timestamp > 200)
        {
            skywire->print(command + "\r");
            _sent = true;
            _sent_timestamp = now;

            state = State::WAIT_SET;
        }

        break;

    case State::WAIT_SET:
        serialReadToRxBuffer();

        if (okReceived())
        {
            state = State::SEND_QUERY;
            _sent = false;
            _rx_buffer = "";
        }
        break;

    case State::SEND_QUERY:
        if (!_sent)
        {
            skywire->print("AT+CGDCONT?\r");
            _sent = true;
            _sent_timestamp = now;
        }

        state = State::WAIT_QUERY;
        break;

    case State::WAIT_QUERY:
        serialReadToRxBuffer();

        const bool has_hologram_ok = (_rx_buffer.indexOf("hologram") != -1 && _rx_buffer.indexOf("OK") != -1);
        const bool already_active = (_rx_buffer.indexOf("context already activated") != -1);
        if (has_hologram_ok || already_active)
        {
            state = State::DONE;

            if (on_completed_function && !_on_completed_called)
            {
                on_completed_function(_rx_buffer);
                _on_completed_called = true;
            }

            return SkywireResponseResult_t(true, _rx_buffer);
        }

        if (okReceived())
        {
            reset();
            state = State::SEND_SET;
        }
        break;

    case State::DONE:
        return SkywireResponseResult_t(true, _rx_buffer);
    }

    if (_sent && now - _sent_timestamp >= 1000)
    {
        if (state == State::WAIT_SET)
        {
            skywire->print(command + "\r");
        }
        else if (state == State::WAIT_QUERY)
        {
            skywire->print("AT+CGDCONT?\r");
        }
        _sent_timestamp = now;
    }

    return SkywireResponseResult_t(false, "");
}
