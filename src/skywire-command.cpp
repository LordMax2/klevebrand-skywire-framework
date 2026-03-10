#include "skywire-command.h"

char SkywireCommand::_rx_buffer[512] = {0};

SkywireCommand::SkywireCommand(HardwareSerial *skywire, const char command[256], bool debug_mode, OnCompletedFunction on_completed_function)
    : skywire(skywire), debug_mode(debug_mode), on_completed_function(on_completed_function)
{
    strncpy(this->command, command, sizeof(this->command) - 1);
    this->command[sizeof(this->command) - 1] = '\0';
}

char* SkywireCommand::getRxBuffer()
{
    return _rx_buffer;
}

void SkywireCommand::appendToRxBuffer(char c)
{
    if (_rx_buffer_cursor_index < sizeof(_rx_buffer) - 1)
    {
        _rx_buffer[_rx_buffer_cursor_index++] = c;
        _rx_buffer[_rx_buffer_cursor_index] = '\0';
    }
    else
    {
        Serial.println("RX buffer overflow. Character not appended.");
    }
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
            skywire->print(command);
            skywire->print("\r");

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
            Serial.println("STEPPER CLIENT STEP: " + String(command) + " RECEIVED OK, RX BUFFER:");
            Serial.println(rx_buffer);
            Serial.println("--- END OF RX BUFFER ---");
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
    _rx_buffer_cursor_index = 0;
    _rx_buffer[0] = '\0';
}

bool SkywireCommand::okReceived()
{
    return strstr(_rx_buffer, "OK") != nullptr;
}

void SkywireCommand::reset()
{
    _sent = false;
    _sent_timestamp = 0;
    _on_completed_called = false;
    _is_completed = false;
    _first_process_call = true;
    _first_process_call_timestamp = 0;

    resetRxBuffer();
}

bool SkywireCommand::completed()
{
    return (_sent && okReceived()) || _is_completed;
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

void SkywireCommand::setCompleted(bool completed)
{
    _is_completed = completed;

    resetRxBuffer();
}