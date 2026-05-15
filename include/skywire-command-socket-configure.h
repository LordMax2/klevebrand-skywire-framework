#ifndef SKYWIRE_COMMAND_SOCKET_CONFIGURE_H
#define SKYWIRE_COMMAND_SOCKET_CONFIGURE_H

#include "skywire-command.h"

class SocketConfigureSkywireCommand : public SkywireCommand
{
public:
    SocketConfigureSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process() override;
    void reset() override;

private:
    enum class State
    {
        SEND_CONFIGURE,
        WAIT_CONFIGURE,
        SEND_CLOSE,
        WAIT_CLOSE
    };

    bool socketSetupFailed() const;
    bool socketCloseFinished() const;

    State state = State::SEND_CONFIGURE;
    unsigned long recovery_started_timestamp = 0;
};

#endif
