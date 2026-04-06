#include "skywire-command-socket-connect.h"

SocketConnectSkywireCommand::SocketConnectSkywireCommand(HardwareSerial *skywire,
                                                         const char *host,
                                                         const int port,
                                                         const bool debug_mode,
                                                         const OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire,
                     ("AT#SD=1,0," + String(port) + ",\"" + String(host) + "\",0,0,1").c_str(),
                     debug_mode,
                     on_completed_function) {
}
