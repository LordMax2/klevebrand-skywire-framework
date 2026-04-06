#ifndef SKYWIRE_COMMAND_SOCKET_SEND_H
#define SKYWIRE_COMMAND_SOCKET_SEND_H

#include "skywire-command.h"

#define SOCKET_SEND_MESSAGE_SIZE 160

class SocketSendSkywireCommand : public SkywireCommand
{
public:
    SocketSendSkywireCommand(HardwareSerial *skywire,
                             bool debug_mode,
                             const char *message,
                             bool read_response,
                             OnCompletedFunction on_completed_function);

    SkywireResponseResult_t process() override;
    bool completed() override;
    void reset() override;

    void setMessage(const char *message_to_send);
    char *getMessage();

private:
    bool promptReceived() const;
    bool responseReceived() const;
    void readSocketResponse();

    char message[SOCKET_SEND_MESSAGE_SIZE]{};
    bool read_response = false;
    bool payload_sent = false;
    bool response_requested = false;
    bool response_received = false;
    unsigned long last_read_timestamp = 0;
};

#endif
