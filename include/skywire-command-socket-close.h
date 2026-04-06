#ifndef SKYWIRE_COMMAND_SOCKET_CLOSE_H
#define SKYWIRE_COMMAND_SOCKET_CLOSE_H

#include "skywire-command.h"

class SocketCloseSkywireCommand : public SkywireCommand
{
public:
    SocketCloseSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);
};

#endif
