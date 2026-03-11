#include "skywire-command-httpsnd.h"

HttpSndSkywireCommand::HttpSndSkywireCommand(HardwareSerial *skywire, bool debug_mode, const char path[48],
                                             const OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "AT#HTTPSND=0,0,", debug_mode, on_completed_function) {
    strncpy(this->path, path, sizeof(this->path) - 1);
    this->path[sizeof(this->path) - 1] = '\0';
}

bool HttpSndSkywireCommand::arrowsReceived() {
    auto rx_buffer = getRxBuffer();

    return strstr(rx_buffer, ">>>") != nullptr || millis() - getSentTimestamp() > 200;
}

void HttpSndSkywireCommand::setPayload(char payload[128]) {
    strncpy(this->payload, payload, sizeof(this->payload) - 1);
    this->payload[sizeof(this->payload) - 1] = '\0';
}

char *HttpSndSkywireCommand::getPayload() {
    return payload;
}

void HttpSndSkywireCommand::reset() {
    SkywireCommand::reset();
    payload_sent = false;
}

SkywireResponseResult_t HttpSndSkywireCommand::process() {
    auto rx_buffer = getRxBuffer();

    if (completed()) {
        return {true, rx_buffer};
    }

    const String payload_to_send = getPayload();
    const unsigned long now = millis();

    setFirstProcessCall();

    if (!isSent()) {
        if (getFirstProcessCallTimestamp() > 200 && getFirstProcessCallTimestamp() != 0) {
            if (debug_mode) {
                Serial.print(command);
                Serial.print(path);
                Serial.print(",");
                Serial.print(payload_to_send.length());
                Serial.print("\r");
            }
            skywire->print(command);
            skywire->print(path);
            skywire->print(",");
            skywire->print(payload_to_send.length());
            skywire->print("\r");

            setSent(true);
        }

        return {false, ""};
    }

    if (!payload_sent) {
        serialReadToRxBuffer();
    }

    if (isSent() && now - getSentTimestamp() > 500 && !payload_sent) {
        if (debug_mode) {
            Serial.println("HTTPSND Sending payload: " + payload_to_send);
        }
        skywire->print(payload_to_send + "\x1A");

        payload_sent = true;

        setCompleted(true);

        if (debug_mode) {
            Serial.println("HTTPSND is completed: " + String(completed()));
            Serial.println(rx_buffer);
        }
    }

    const bool is_complete = completed();
    if (is_complete) {
        setCompleted(true);

        if (on_completed_function != nullptr && !isOnCompletedCalled()) {
            on_completed_function(rx_buffer);
            setOnCompletedCalled(true);
        }
    }

    return {false, ""};
}

bool HttpSndSkywireCommand::completed() {
    return _is_completed || ((isSent() && okReceived() && arrowsReceived() && payload_sent) || getPayload()[0] == '\0');
}
