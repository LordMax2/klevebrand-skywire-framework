#ifndef SKYWIRE_COMMAND_SOCKET_CONNECT_H
#define SKYWIRE_COMMAND_SOCKET_CONNECT_H

#include "skywire-command.h"

#define SOCKET_CONNECT_COMMAND_SIZE 64

class SocketConnectSkywireCommand : public SkywireCommand
{
public:
    SocketConnectSkywireCommand(HardwareSerial *skywire,
                                const char *host,
                                int port,
                                bool debug_mode,
                                OnCompletedFunction on_completed_function);

protected:
    void writeCommandToModem() override;

private:
    char command_buffer[SOCKET_CONNECT_COMMAND_SIZE]{};
};

#endif
