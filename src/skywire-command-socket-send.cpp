#include "skywire-command-socket-send.h"

SocketSendSkywireCommand::SocketSendSkywireCommand(HardwareSerial *skywire,
                                                   const bool debug_mode,
                                                   const char *message,
                                                   const bool read_response,
                                                   const OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "AT#SSEND=1", debug_mode, on_completed_function),
      read_response(read_response) {
    setMessage(message);
}

void SocketSendSkywireCommand::setMessage(const char *message_to_send) {
    strncpy(this->message, message_to_send, sizeof(this->message) - 1);
    this->message[sizeof(this->message) - 1] = '\0';
}

char *SocketSendSkywireCommand::getMessage() {
    return message;
}

bool SocketSendSkywireCommand::promptReceived() const {
    return strstr(getRxBuffer(), ">") != nullptr;
}

bool SocketSendSkywireCommand::responseReceived() const {
    const auto rx_buffer = getRxBuffer();
    return strstr(rx_buffer, "#SRECV:") != nullptr && strstr(rx_buffer, "\r\nOK\r\n") != nullptr;
}

void SocketSendSkywireCommand::readSocketResponse() {
    skywire->print("AT#SRECV=1,255\r");
    last_read_timestamp = millis();
    response_requested = true;

    if (debug_mode) {
        Serial.println(F("AT#SRECV=1,255"));
    }
}

SkywireResponseResult_t SocketSendSkywireCommand::process() {
    auto rx_buffer = getRxBuffer();

    if (completed()) {
        return {true, rx_buffer};
    }

    setFirstProcessCall();
    const unsigned long now = millis();

    if (!isSent()) {
        if (now - getFirstProcessCallTimestamp() > 200 && getFirstProcessCallTimestamp() != 0) {
            resetRxBuffer();
            skywire->print(command);
            skywire->print("\r");

            if (debug_mode) {
                Serial.println("AT#SSEND=1");
            }

            setSent(true);
        }

        return {false, ""};
    }

    serialReadToRxBuffer();
    rx_buffer = getRxBuffer();

    if (!payload_sent) {
        if (!promptReceived()) {
            return {false, ""};
        }

        skywire->print(getMessage());
        skywire->write(0x1A);
        payload_sent = true;

        if (debug_mode) {
            Serial.print(F("Socket payload: "));
            Serial.println(getMessage());
        }

        resetRxBuffer();
        return {false, ""};
    }

    if (!read_response) {
        if (!okReceived()) {
            return {false, ""};
        }
    } else {
        if (!response_requested && okReceived()) {
            resetRxBuffer();
            readSocketResponse();
            return {false, ""};
        }

        if (response_requested && !response_received && now - last_read_timestamp > 200 && !skywire->available()) {
            readSocketResponse();
        }

        if (!responseReceived()) {
            return {false, ""};
        }

        response_received = true;
    }

    if (on_completed_function != nullptr && !isOnCompletedCalled()) {
        on_completed_function(rx_buffer);
        setOnCompletedCalled(true);
    }

    setCompleted(true);
    return {true, rx_buffer};
}

bool SocketSendSkywireCommand::completed() {
    if (message[0] == '\0') {
        return true;
    }

    if (read_response) {
        return _is_completed || response_received;
    }

    return _is_completed || (payload_sent && okReceived());
}

void SocketSendSkywireCommand::reset() {
    SkywireCommand::reset();
    payload_sent = false;
    response_requested = false;
    response_received = false;
    last_read_timestamp = 0;
}
