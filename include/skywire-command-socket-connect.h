#ifndef SKYWIRE_COMMAND_SOCKET_CONNECT_H
#define SKYWIRE_COMMAND_SOCKET_CONNECT_H

#include "skywire-command.h"

class SocketConnectSkywireCommand : public SkywireCommand
{
public:
    SocketConnectSkywireCommand(HardwareSerial *skywire,
                                const char *host,
                                int port,
                                bool debug_mode,
                                OnCompletedFunction on_completed_function);
};

#endif
