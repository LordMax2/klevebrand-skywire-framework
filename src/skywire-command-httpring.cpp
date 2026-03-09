#include "skywire-command-httpring.h"

HttpRingSkywireCommand::HttpRingSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String& result_content))
    : SkywireCommand(skywire, "HTTPRING", debug_mode, on_completed_function)
{
}

SkywireResponseResult_t HttpRingSkywireCommand::process()
{
    auto rx_buffer = getRxBuffer();

    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    const unsigned long now = millis();

    setFirstProcessCall();

    if(!isSent()) setSent(true);

    serialReadToRxBuffer();

    const bool has_ok = okReceived();
    if (debug_mode && has_ok)
    {
        Serial.println(rx_buffer);
        Serial.println("STEPPER CLIENT RECEIVED HTTPRING OK: " + rx_buffer);
    }

    return SkywireResponseResult_t(false, "");
}

bool HttpRingSkywireCommand::okReceived()
{
    auto rx_buffer = getRxBuffer();

    return rx_buffer.indexOf("HTTPRING") != -1 && rx_buffer.indexOf("\r\n") != -1;
}

bool HttpRingSkywireCommand::completed()
{
    auto rx_buffer = getRxBuffer();

    return isSent() && okReceived() && (millis() - getSentTimestamp()) > 500;
}
