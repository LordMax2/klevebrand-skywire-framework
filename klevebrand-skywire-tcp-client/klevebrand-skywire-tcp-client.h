#ifndef KLEVEBRAND_SKYWIRE_TCP_CLIENT_H
#define KLEVEBRAND_SKYWIRE_TCP_CLIENT_H

#include "klevebrand-skywire-framework.h"

class SkywireTcpClient {
  public: 
    SkywireTcpClient(String ipAddress, int port);
    void open();
    void send(String payload);
    void close();
    Skywire skywire;
  private: 
    String ipAddress;
    int port;
};

#endif //KLEVEBRAND_SKYWIRE_TCP_CLIENT_H