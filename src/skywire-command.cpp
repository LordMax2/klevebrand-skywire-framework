#include "skywire-command.h"

char SkywireCommand::_rx_buffer[SKYWIRE_RX_BUFFER_SIZE] = {0};

SkywireCommand::SkywireCommand(HardwareSerial *skywire, const char command[COMMAND_SIZE], const bool debug_mode, const OnCompletedFunction on_completed_function)
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
        Serial.println(F("RX buffer overflow. Character not appended."));
        Serial.print(F("Size: "));
        Serial.println(sizeof(_rx_buffer) - 1);
    }
}

void SkywireCommand::serialReadToRxBuffer()
{
    while (skywire->available())
    {
        const char c = skywire->read();
        appendToRxBuffer(c);

        if (debug_mode)
        {
            const uint8_t byte_value = static_cast<uint8_t>(c);
            Serial.print("RX BYTE 0x");
            if (byte_value < 0x10)
            {
                Serial.print("0");
            }
            Serial.print(byte_value, HEX);
            Serial.print(" '");
            if (byte_value == '\r')
            {
                Serial.print("CR");
            }
            else if (byte_value == '\n')
            {
                Serial.print("LF");
            }
            else if (byte_value == '\t')
            {
                Serial.print("TAB");
            }
            else if (byte_value >= 32 && byte_value <= 126)
            {
                Serial.print(static_cast<char>(byte_value));
            }
            else
            {
                Serial.print(".");
            }
            Serial.println("'");
        }
    }
}

SkywireResponseResult_t SkywireCommand::process()
{
    auto rx_buffer = getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    const unsigned long now = millis();

    setFirstProcessCall();

    if (!isSent())
    {
        if ((now - getFirstProcessCallTimestamp() > 200 && getFirstProcessCallTimestamp() != 0))
        {
            resetRxBuffer();
            skywire->print(command);
            skywire->print("\r");

            if (debug_mode) {
                Serial.println(command);
            }

            setSent(true);
        }

        return {false, ""};
    }

    serialReadToRxBuffer();

    rx_buffer = getRxBuffer();

    const bool has_ok = okReceived();

    if (has_ok)
    {
        if (debug_mode)
        {
            Serial.print(F("STEPPER CLIENT STEP: "));
            Serial.print(command);
            Serial.println(F(" RECEIVED OK, RX BUFFER:"));
            Serial.println(rx_buffer);
            Serial.println(F("--- END OF RX BUFFER ---"));
        }

        const bool is_complete = completed();
        if (is_complete)
        {
            if(on_completed_function != nullptr && !isOnCompletedCalled())
            {
                on_completed_function(rx_buffer);
                setOnCompletedCalled(true);
            }

            setCompleted(true);
        }

        return {true, rx_buffer};
    }

    return {false, ""};
}

void SkywireCommand::resetRxBuffer()
{
    _rx_buffer_cursor_index = 0;
    _rx_buffer[0] = '\0';
}

bool SkywireCommand::okReceived()
{
    return strstr(_rx_buffer, "\r\nOK\r\n") != nullptr;
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
    return _is_completed || (_sent && okReceived());
}

void SkywireCommand::setFirstProcessCall()
{
    if (_first_process_call)
    {
        _first_process_call_timestamp = millis();
        _first_process_call = false;
    }
}

bool SkywireCommand::isFirstProcessCalled() const
{
    return !_first_process_call;
}

unsigned long SkywireCommand::getFirstProcessCallTimestamp() const
{
    return _first_process_call_timestamp;
}

void SkywireCommand::setSent(const bool sent)
{
    _sent = sent;
    if (sent)
    {
        _sent_timestamp = millis();
    }
}

bool SkywireCommand::isSent() const
{
    return _sent;
}

unsigned long SkywireCommand::getSentTimestamp() const
{
    return _sent_timestamp;
}

bool SkywireCommand::isOnCompletedCalled() const
{
    return _on_completed_called;
}

void SkywireCommand::setOnCompletedCalled(const bool on_completed_called)
{
    _on_completed_called = on_completed_called;
}

void SkywireCommand::setCompleted(const bool completed)
{
    _is_completed = completed;
}
