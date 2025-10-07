#ifndef KLEVEBRAND_SKYWIRE_TCP_CLIENT_H
#define KLEVEBRAND_SKYWIRE_TCP_CLIENT_H

#include "../klevebrand-skywire-framework/klevebrand-skywire-framework.h"

class SkywireTcpClient
{
public:
  SkywireTcpClient(HardwareSerial skywire_serial, String ip_address, int port);
  bool open();
  bool send(String message);
  bool close();
  int available();
  String readString();
  Skywire skywire;

private:
  String ip_address;
  int port;
  int socket_dial_connection_id;
};

#endif // KLEVEBRAND_SKYWIRE_TCP_CLIENT_H