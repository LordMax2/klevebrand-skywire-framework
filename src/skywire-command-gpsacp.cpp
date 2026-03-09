#include "skywire-command-gpsacp.h"

GpsAcpSkywireCommand::GpsAcpSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String& result_content))
    : SkywireCommand(skywire, "", debug_mode, on_completed_function)
{
}

SkywireResponseResult_t GpsAcpSkywireCommand::process()
{
    if (completed())
    {
        return SkywireResponseResult_t(true, _rx_buffer);
    }

    const unsigned long now = millis();

    if (_first_process_call)
    {
        _first_process_call = false;
        _first_process_call_timestamp = now;
    }

    if (!_sent && now - _first_process_call_timestamp > 200)
    {
        skywire->print("AT$GPSACP\r");

        _sent = true;
        _sent_timestamp = now;

        return SkywireResponseResult_t(false, "");
    }

    serialReadToRxBuffer();

    const bool has_ok = okReceived();
    if (debug_mode && has_ok)
    {
        Serial.println(_rx_buffer);
        Serial.println("STEPPER CLIENT RECEIVED GPSACP");
    }

    const bool is_complete = completed();
    if (is_complete && on_completed_function != nullptr && !_on_completed_called)
    {
        on_completed_function(_rx_buffer);

        _on_completed_called = true;
    }

    return SkywireResponseResult_t(false, "");
}
