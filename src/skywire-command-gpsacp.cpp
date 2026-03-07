#include "skywire-command-gpsacp.h"

GpsAcpSkywireCommand::GpsAcpSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String& result_content))
    : SkywireCommand(skywire, "", debug_mode, on_completed_function)
{
}

SkywireResponseResult_t GpsAcpSkywireCommand::process()
{
    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    if (!sent)
    {
        skywire->print("AT$GPSACP\r");

        sent = true;

        sent_timestamp = millis();

        return SkywireResponseResult_t(false, "");
    }

    serialReadToRxBuffer();

    if (debug_mode && okReceived())
    {
        Serial.println(rx_buffer);
        Serial.println("STEPPER CLIENT RECEIVED GPSACP");
    }

    if (completed() && on_completed_function != nullptr && !on_completed_called)
    {
        on_completed_function(rx_buffer);

        on_completed_called = true;
    }

    return SkywireResponseResult_t(false, "");
}
