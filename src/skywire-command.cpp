#include "skywire-command.h"

SkywireCommand::SkywireCommand(HardwareSerial *skywire, char* command, bool debug_mode, void (*on_completed_function)(char* result_content))
    : skywire(skywire), command(command), debug_mode(debug_mode), on_completed_function(on_completed_function)
{
}

char* SkywireCommand::getRxBuffer()
{
    return _rx_buffer;
}

void SkywireCommand::appendToRxBuffer(char c)
{
    _rx_buffer += c;
}

void SkywireCommand::serialReadToRxBuffer()
{
    while (skywire->available())
    {
        char c = skywire->read();
        appendToRxBuffer(c);
    }
}

SkywireResponseResult_t SkywireCommand::process()
{
    auto rx_buffer = getRxBuffer();

    if (completed())
    {
        return SkywireResponseResult_t(true, rx_buffer);
    }

    const unsigned long now = millis();

    setFirstProcessCall();

    if (!isSent())
    {
        if ((now - getFirstProcessCallTimestamp() > 200 && getFirstProcessCallTimestamp() != 0))
        {
            skywire->print(command + "\r");

            setSent(true);
        }

        return SkywireResponseResult_t(false, "");
    }

    serialReadToRxBuffer();

    rx_buffer = getRxBuffer();

    const bool has_ok = okReceived();

    if (has_ok)
    {
        if (debug_mode)
        {
            Serial.println(rx_buffer);
            Serial.println("STEPPER CLIENT STEP: " + command + "RECEIVED OK");
        }

        const bool is_complete = completed();
        if (is_complete && on_completed_function != nullptr && !isOnCompletedCalled())
        {
            on_completed_function(rx_buffer);

            setOnCompletedCalled(true);
        }

        return SkywireResponseResult_t(true, rx_buffer);
    }

    return SkywireResponseResult_t(false, "");
}

void SkywireCommand::resetRxBuffer()
{
    _rx_buffer = "";
}

bool SkywireCommand::okReceived()
{
    return _rx_buffer.indexOf("OK") != -1;
}

void SkywireCommand::reset()
{
    _sent = false;
    _sent_timestamp = 0;
    _on_completed_called = false;
    _first_process_call = true;
    _first_process_call_timestamp = 0;

    resetRxBuffer();
}

bool SkywireCommand::completed()
{
    return _sent && okReceived();
}

void SkywireCommand::setFirstProcessCall()
{
    if (_first_process_call)
    {
        _first_process_call_timestamp = millis();
        _first_process_call = false;
    }
}

bool SkywireCommand::isFirstProcessCalled()
{
    return !_first_process_call;
}

unsigned long SkywireCommand::getFirstProcessCallTimestamp()
{
    return _first_process_call_timestamp;
}

void SkywireCommand::setSent(bool sent)
{
    _sent = sent;
    if (sent)
    {
        _sent_timestamp = millis();
    }
}

bool SkywireCommand::isSent()
{
    return _sent;
}

unsigned long SkywireCommand::getSentTimestamp()
{
    return _sent_timestamp;
}

bool SkywireCommand::isOnCompletedCalled()
{
    return _on_completed_called;
}

void SkywireCommand::setOnCompletedCalled(bool on_completed_called)
{
    _on_completed_called = on_completed_called;
}