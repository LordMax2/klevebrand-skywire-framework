#include "skywire-command-httpring.h"

HttpRingSkywireCommand::HttpRingSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String& result_content))
    : SkywireCommand(skywire, "HTTPRING", debug_mode, on_completed_function)
{
}

SkywireResponseResult_t HttpRingSkywireCommand::process()
{
    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    const unsigned long now = millis();

    if (!sent)
    {
        sent = true;
        sent_timestamp = now;
    }

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
    return rx_buffer.indexOf("HTTPRING") != -1 && rx_buffer.indexOf("\r\n") != -1;
}

bool HttpRingSkywireCommand::completed()
{
    return sent && okReceived() && (millis() - sent_timestamp) > 500;
}
