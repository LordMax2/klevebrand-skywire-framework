#include "skywire-command-socket-connect.h"

SocketConnectSkywireCommand::SocketConnectSkywireCommand(HardwareSerial *skywire,
                                                         const char *host,
                                                         const int port,
                                                         const bool debug_mode,
                                                         const OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "", debug_mode, on_completed_function) {
    snprintf(command, sizeof(command), "AT#SD=1,0,%d,\"%s\",0,0,1", port, host);
}
