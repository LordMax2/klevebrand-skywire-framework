#include "skywire-command-socket-configure.h"

SocketConfigureSkywireCommand::SocketConfigureSkywireCommand(HardwareSerial *skywire,
                                                             const bool debug_mode,
                                                             const OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, F("AT#SCFG=1,1,300,90,600,50"), debug_mode, on_completed_function) {
}

bool SocketConfigureSkywireCommand::socketSetupFailed() const {
    return strstr(getRxBuffer(), "+CME ERROR: can not setup socket") != nullptr;
}

bool SocketConfigureSkywireCommand::socketCloseFinished() const {
    const auto rx_buffer = getRxBuffer();
    return strstr(rx_buffer, "\r\nOK\r\n") != nullptr ||
           strstr(rx_buffer, "ERROR") != nullptr ||
           strstr(rx_buffer, "+CME ERROR") != nullptr;
}

SkywireResponseResult_t SocketConfigureSkywireCommand::process() {
    auto rx_buffer = getRxBuffer();

    if (completed()) {
        return {true, rx_buffer};
    }

    const unsigned long now = millis();
    setFirstProcessCall();

    switch (state) {
        case State::SEND_CONFIGURE:
            if (!isSent() && now - getFirstProcessCallTimestamp() > 200 && getFirstProcessCallTimestamp() != 0) {
                resetRxBuffer();
                writeCommandToModem();
                setSent(true);
                state = State::WAIT_CONFIGURE;
            }
            return {false, ""};

        case State::WAIT_CONFIGURE:
            serialReadToRxBuffer();
            rx_buffer = getRxBuffer();

            if (socketSetupFailed()) {
                if (debug_mode) {
                    Serial.println(F("Socket configure failed, closing socket before retry."));
                }

                resetRxBuffer();
                setSent(false);
                recovery_started_timestamp = now;
                state = State::SEND_CLOSE;
                return {false, ""};
            }

            if (!okReceived()) {
                return {false, ""};
            }

            if (on_completed_function != nullptr && !isOnCompletedCalled()) {
                on_completed_function(rx_buffer);
                setOnCompletedCalled(true);
            }

            setCompleted(true);
            return {true, rx_buffer};

        case State::SEND_CLOSE:
            if (!isSent()) {
                skywire->print(F("AT#SH=1\r"));

                if (debug_mode) {
                    Serial.println(F("AT#SH=1"));
                }

                setSent(true);
                state = State::WAIT_CLOSE;
            }
            return {false, ""};

        case State::WAIT_CLOSE:
            serialReadToRxBuffer();

            if (!socketCloseFinished()) {
                if (recovery_started_timestamp != 0 && now - recovery_started_timestamp > 1000) {
                    resetRxBuffer();
                    setSent(false);
                    recovery_started_timestamp = now;
                    state = State::SEND_CLOSE;
                }

                return {false, ""};
            }

            resetRxBuffer();
            setSent(false);
            recovery_started_timestamp = 0;
            state = State::SEND_CONFIGURE;
            return {false, ""};
    }

    return {false, ""};
}

void SocketConfigureSkywireCommand::reset() {
    SkywireCommand::reset();
    state = State::SEND_CONFIGURE;
    recovery_started_timestamp = 0;
}
