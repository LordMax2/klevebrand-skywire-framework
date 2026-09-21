#include "skywire_at_engine.h"
#include "skywire_strstr_p.h"

HardwareSerial *SkywireAtEngine::_skywire = nullptr;
bool SkywireAtEngine::_debug_mode = false;
char SkywireAtEngine::_rx_buffer[SKYWIRE_RX_BUFFER_SIZE] = {0};
size_t SkywireAtEngine::_rx_buffer_cursor_index = 0;

SkywireAtEngine::SkywireAtEngine(
    HardwareSerial *skywire,
    const __FlashStringHelper *command,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _command(command),
      _on_completed_function(on_completed_function),
      _sent_timestamp(0),
      _first_process_call_timestamp(0),
      _flags(0)
{
    _skywire = skywire;
    _debug_mode = debug_mode;
}

bool SkywireAtEngine::flag(const uint8_t mask) const
{
    return (_flags & mask) != 0;
}

void SkywireAtEngine::setFlag(const uint8_t mask, const bool value)
{
    if (value)
    {
        _flags = static_cast<uint8_t>(_flags | mask);
    }
    else
    {
        _flags = static_cast<uint8_t>(_flags & static_cast<uint8_t>(~mask));
    }
}

const __FlashStringHelper *SkywireAtEngine::command() const
{
    return _command;
}

bool SkywireAtEngine::debugMode()
{
    return _debug_mode;
}

void SkywireAtEngine::rebeginModem()
{
    if (_skywire != nullptr)
    {
        _skywire->begin(115200);
    }
}

void SkywireAtEngine::logStepTimeout(
    const __FlashStringHelper *command,
    const unsigned long timeout_milliseconds,
    const unsigned long sent_timestamp)
{
    Serial.print(F("Skywire command step: "));
    Serial.print(command);
    Serial.print(F(", after "));
    Serial.print(timeout_milliseconds);
    Serial.print(F("ms, restarting sequence. Sent timestamp: "));
    Serial.print(sent_timestamp);
    Serial.print(F(", current timestamp: "));
    Serial.println(millis());
    Serial.print(F("rx_buffer at timeout: ["));
    Serial.print(getRxBuffer());
    Serial.println(F("]"));
}

char *SkywireAtEngine::getRxBuffer()
{
    return _rx_buffer;
}

void SkywireAtEngine::resetRxBuffer()
{
    _rx_buffer_cursor_index = 0;
    _rx_buffer[0] = '\0';
}

void SkywireAtEngine::serialReadToRxBuffer()
{
    while (_skywire->available())
    {
        const char c = static_cast<char>(_skywire->read());

        if (_rx_buffer_cursor_index < sizeof(_rx_buffer) - 1)
        {
            _rx_buffer[_rx_buffer_cursor_index++] = c;
            _rx_buffer[_rx_buffer_cursor_index] = '\0';
        }
        else if (_debug_mode)
        {
            Serial.println(F("RX buffer overflow. Character not appended."));
            Serial.print(F("Size: "));
            Serial.println(sizeof(_rx_buffer) - 1);
        }

        if (_debug_mode)
        {
            const uint8_t byte_value = static_cast<uint8_t>(c);

            Serial.print(F("RX BYTE 0x"));
            if (byte_value < 0x10)
            {
                Serial.print(F("0"));
            }

            Serial.print(byte_value, HEX);
            Serial.print(F(" '"));
            if (byte_value == '\r')
            {
                Serial.print(F("CR"));
            }
            else if (byte_value == '\n')
            {
                Serial.print(F("LF"));
            }
            else if (byte_value == '\t')
            {
                Serial.print(F("TAB"));
            }
            else if (byte_value >= 32 && byte_value <= 126)
            {
                Serial.print(static_cast<char>(byte_value));
            }
            else
            {
                Serial.print(F("."));
            }

            Serial.println(F("'"));
        }
    }
}

void SkywireAtEngine::writeCommandToModem()
{
    _skywire->print(_command);
    _skywire->print('\r');

    if (_debug_mode)
    {
        Serial.println(_command);
    }
}

void SkywireAtEngine::printToModem(const __FlashStringHelper *value)
{
    _skywire->print(value);
}

void SkywireAtEngine::printToModem(const char *value)
{
    _skywire->print(value);
}

void SkywireAtEngine::printToModem(const char value)
{
    _skywire->print(value);
}

void SkywireAtEngine::printToModem(const int value)
{
    _skywire->print(value);
}

void SkywireAtEngine::writeToModem(const uint8_t value)
{
    _skywire->write(value);
}

bool SkywireAtEngine::modemAvailable() const
{
    return _skywire->available() > 0;
}

bool SkywireAtEngine::okReceived() const
{
    return skywireContainsP(_rx_buffer, PSTR("\r\nOK\r\n")) ||
           skywireContainsP(_rx_buffer, PSTR("+CME ERROR: context already activated"));
}

void SkywireAtEngine::setFirstProcessCall()
{
    if (!flag(kFlagFirstProcessCalled))
    {
        _first_process_call_timestamp = millis();
        setFlag(kFlagFirstProcessCalled, true);
    }
}

unsigned long SkywireAtEngine::getFirstProcessCallTimestamp() const
{
    return _first_process_call_timestamp;
}

void SkywireAtEngine::setSent(const bool sent)
{
    setFlag(kFlagSent, sent);
    if (sent)
    {
        _sent_timestamp = millis();
    }
}

bool SkywireAtEngine::isSent() const
{
    return flag(kFlagSent);
}

unsigned long SkywireAtEngine::getSentTimestamp() const
{
    return _sent_timestamp;
}

void SkywireAtEngine::setCompleted(const bool completed)
{
    setFlag(kFlagCompleted, completed);
}

bool SkywireAtEngine::isCompletedFlag() const
{
    return flag(kFlagCompleted);
}

bool SkywireAtEngine::isOnCompletedCalled() const
{
    return flag(kFlagOnCompletedCalled);
}

void SkywireAtEngine::setOnCompletedCalled(const bool on_completed_called)
{
    setFlag(kFlagOnCompletedCalled, on_completed_called);
}

void SkywireAtEngine::notifyCompletedIfNeeded()
{
    if (_on_completed_function != nullptr && !isOnCompletedCalled())
    {
        _on_completed_function(getRxBuffer());
        setOnCompletedCalled(true);
    }
}

bool SkywireAtEngine::completed() const
{
    return isCompletedFlag() || (isSent() && okReceived());
}

void SkywireAtEngine::reset()
{
    _sent_timestamp = 0;
    _first_process_call_timestamp = 0;
    _flags = 0;
    resetRxBuffer();
}

bool SkywireAtEngine::waitForSendThenRead()
{
    const unsigned long now = millis();

    setFirstProcessCall();

    if (!isSent())
    {
        if (now - getFirstProcessCallTimestamp() > 200 && getFirstProcessCallTimestamp() != 0)
        {
            resetRxBuffer();
            writeCommandToModem();
            setSent(true);
        }

        return false;
    }

    serialReadToRxBuffer();

    return true;
}

SkywireResponseResult_t SkywireAtEngine::process()
{
    char *const rx_buffer = getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    if (!waitForSendThenRead())
    {
        return {false, rx_buffer};
    }

    const bool has_ok = okReceived();

    if (!has_ok)
    {
        return {false, rx_buffer};
    }

    if (_debug_mode)
    {
        Serial.print(F("STEPPER CLIENT STEP: "));
        Serial.print(_command);
        Serial.println(F(" RECEIVED OK, RX BUFFER:"));
        Serial.println(rx_buffer);
        Serial.println(F("--- END OF RX BUFFER ---"));
    }

    notifyCompletedIfNeeded();
    setCompleted(true);

    return {true, rx_buffer};
}
