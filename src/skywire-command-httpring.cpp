#include "skywire-command-httpring.h"

HttpRingSkywireCommand::HttpRingSkywireCommand(HardwareSerial *skywire, const bool debug_mode,
                                               const OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "HTTPRING", debug_mode, on_completed_function) {
}

SkywireResponseResult_t HttpRingSkywireCommand::process() {
    auto rx_buffer = getRxBuffer();

    if (completed()) {
        return {true, rx_buffer};
    }

    setFirstProcessCall();

    if (!isSent())
        setSent(true);

    serialReadToRxBuffer();

    const bool has_ok = okReceived();
    if (debug_mode && has_ok) {
        Serial.println("STEPPER CLIENT RECEIVED HTTPRING OK: ");
        Serial.println(rx_buffer);
        Serial.println("--- END OF RX BUFFER ---");
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

bool HttpRingSkywireCommand::okReceived() {
    const auto rx_buffer = getRxBuffer();

    const char *ring = strstr(rx_buffer, "HTTPRING");
    if (ring == nullptr)
        return false;

    return strchr(ring, '\r') != nullptr;
}

bool HttpRingSkywireCommand::completed() {
    return _is_completed || (isSent() && okReceived() && millis() - getSentTimestamp() > 500);
}
