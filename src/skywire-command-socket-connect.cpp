#include "skywire-command-socket-connect.h"

SocketConnectSkywireCommand::SocketConnectSkywireCommand(HardwareSerial *skywire,
                                                         const char *host,
                                                         const int port,
                                                         const bool debug_mode,
                                                         const OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, F("AT#SD"), debug_mode, on_completed_function) {
    snprintf(command_buffer, sizeof(command_buffer), "AT#SD=1,0,%d,\"%s\",0,0,1", port, host);
}

void SocketConnectSkywireCommand::writeCommandToModem() {
    skywire->print(command_buffer);
    skywire->print('\r');

    if (debug_mode) {
        Serial.println(command_buffer);
    }
}
