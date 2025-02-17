#ifndef KLEVEBRAND_SKYWIRE_TCP_CLIENT_H
#define KLEVEBRAND_SKYWIRE_TCP_CLIENT_H

#include "../klevebrand-skywire-framework.h"

class SkywireTcpClient {
  public: 
    SkywireTcpClient(String ipAddress, int port);
    bool open();
    bool send(String message);
    bool close();
    int available();
    String readString();
    size_t write(char *payload);
    Skywire skywire;
  private: 
    String ipAddress;
    int port;
};

#endif //KLEVEBRAND_SKYWIRE_TCP_CLIENT_H