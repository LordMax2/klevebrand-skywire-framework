#include "skywire-command-socket-connect.h"

SocketConnectSkywireCommand::SocketConnectSkywireCommand(
    HardwareSerial *skywire,
    const char *host,
    const int port,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#SD"), debug_mode, on_completed_function),
      _host(host),
      _port(port)
{
}

void SocketConnectSkywireCommand::writeConnectCommandToModem()
{
    _at.printToModem(F("AT#SD=1,0,"));
    _at.printToModem(_port);
    _at.printToModem(F(",\""));
    _at.printToModem(_host != nullptr ? _host : "");
    _at.printToModem(F("\",0,0,1\r"));

    if (SkywireAtEngine::debugMode())
    {
        Serial.print(F("AT#SD=1,0,"));
        Serial.print(_port);
        Serial.print(F(",\""));
        Serial.print(_host != nullptr ? _host : "");
        Serial.println(F("\",0,0,1"));
    }
}

SkywireResponseResult_t SocketConnectSkywireCommand::process()
{
    char *const rx_buffer = SkywireAtEngine::getRxBuffer();

    if (_at.completed())
    {
        return {true, rx_buffer};
    }

    _at.setFirstProcessCall();

    if (!_at.isSent())
    {
        if (millis() - _at.getFirstProcessCallTimestamp() > 200 && _at.getFirstProcessCallTimestamp() != 0)
        {
            _at.resetRxBuffer();
            writeConnectCommandToModem();
            _at.setSent(true);
        }

        return {false, rx_buffer};
    }

    _at.serialReadToRxBuffer();

    if (!_at.okReceived())
    {
        return {false, rx_buffer};
    }

    _at.notifyCompletedIfNeeded();
    _at.setCompleted(true);

    return {true, rx_buffer};
}
