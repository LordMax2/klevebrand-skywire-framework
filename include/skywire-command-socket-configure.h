#ifndef SKYWIRE_COMMAND_SOCKET_CONFIGURE_H
#define SKYWIRE_COMMAND_SOCKET_CONFIGURE_H

#include "skywire-command.h"

class SocketConfigureSkywireCommand : public SkywireCommand
{
public:
    SocketConfigureSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);
};

#endif
