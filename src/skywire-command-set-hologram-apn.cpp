#include "skywire-command-set-hologram-apn.h"

SetApnHologramSkywireCommand::SetApnHologramSkywireCommand(HardwareSerial *skywire, bool debug_mode, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "AT+CGDCONT=1,\"IPV4V6\",\"hologram\"", debug_mode, on_completed_function),
      state(State::SEND_SET)
{
}

SkywireResponseResult_t SetApnHologramSkywireCommand::process()
{
    unsigned long now = millis();

    switch (state)
    {
    case State::SEND_SET:
        if (first_process_call)
        {
            first_process_call = false;
            first_process_call_timestamp = millis();
        }

        if (!sent && millis() - first_process_call_timestamp > 200)
        {
            skywire->print(command + "\r");
            sent = true;
            sent_timestamp = now;

            state = State::WAIT_SET;
        }

        break;

    case State::WAIT_SET:
        serialReadToRxBuffer();

        if (okReceived())
        {
            state = State::SEND_QUERY;
            sent = false;
            rx_buffer = "";
        }
        break;

    case State::SEND_QUERY:
        if (!sent)
        {
            skywire->print("AT+CGDCONT?\r");
            sent = true;
            sent_timestamp = now;
        }

        state = State::WAIT_QUERY;
        break;

    case State::WAIT_QUERY:
        serialReadToRxBuffer();

        if ((rx_buffer.indexOf("hologram") != -1 && rx_buffer.indexOf("OK") != -1) || rx_buffer.indexOf("context already activated") != -1)
        {
            state = State::DONE;

            if (on_completed_function && !on_completed_called)
            {
                on_completed_function(rx_buffer);
                on_completed_called = true;
            }

            return SkywireResponseResult_t(true, rx_buffer);
        }

        if (okReceived())
        {
            resetState();

            state = State::SEND_SET;
        }
        break;

    case State::DONE:
        return SkywireResponseResult_t(true, rx_buffer);
    }

    if (sent && now - sent_timestamp >= 1000)
    {
        if (state == State::WAIT_SET)
        {
            skywire->print(command + "\r");
        }
        else if (state == State::WAIT_QUERY)
        {
            skywire->print("AT+CGDCONT?\r");
        }
        sent_timestamp = now;
    }

    return SkywireResponseResult_t(false, "");
}
