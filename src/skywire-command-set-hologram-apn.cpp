#include "skywire-command-set-hologram-apn.h"

SetApnHologramSkywireCommand::SetApnHologramSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "AT+CGDCONT=1,\"IPV4V6\",\"hologram\"", debug_mode, on_completed_function),
      state(State::SEND_SET)
{
}

SkywireResponseResult_t SetApnHologramSkywireCommand::process()
{
    const unsigned long now = millis();

    auto rx_buffer = getRxBuffer();

    switch (state)
    {
    case State::SEND_SET:
        setFirstProcessCall();

        if (!isSent() && millis() - getFirstProcessCallTimestamp() > 200 && getFirstProcessCallTimestamp() != 0)
        {
            skywire->print(command);
            skywire->print('\r');
            setSent(true);

            state = State::WAIT_SET;
        }

        break;

    case State::WAIT_SET:
        serialReadToRxBuffer();

        if (okReceived())
        {
            state = State::SEND_QUERY;

            setSent(false);
            resetRxBuffer();
        }

        break;

    case State::SEND_QUERY:
        if (!isSent())
        {
            skywire->print("AT+CGDCONT?\r");

            setSent(true);
        }

        state = State::WAIT_QUERY;

        break;

    case State::WAIT_QUERY:
    {
        serialReadToRxBuffer();

        rx_buffer = getRxBuffer();

        const bool has_hologram_ok = (strstr(rx_buffer, "hologram") != nullptr && strstr(rx_buffer, "OK") != nullptr);
        const bool already_active = (strstr(rx_buffer, "context already activated") != nullptr);

        if (has_hologram_ok || already_active)
        {
            state = State::DONE;

            if (on_completed_function && !isOnCompletedCalled())
            {
                on_completed_function(rx_buffer);

                setOnCompletedCalled(true);
            }

            return SkywireResponseResult_t(true, rx_buffer);
        }

        if (okReceived())
        {
            reset();
        }
    }
    break;

    case State::DONE:
        return SkywireResponseResult_t(true, rx_buffer);
    }

    if (isSent() && now - getSentTimestamp() >= 1000)
    {
        if (state == State::WAIT_SET)
        {
            skywire->print(command);
            skywire->print('\r');
        }
        else if (state == State::WAIT_QUERY)
        {
            skywire->print("AT+CGDCONT?\r");
        }

        setSent(true);
    }

    return SkywireResponseResult_t(false, "");
}

void SetApnHologramSkywireCommand::reset()
{
    SkywireCommand::reset();

    state = State::SEND_SET;
}
