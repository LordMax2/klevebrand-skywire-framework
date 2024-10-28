#include "klevebrand-skywire-tcp-client.h"
#include "klevebrand-skywire-framework.h"

SkywireTcpClient::SkywireTcpClient(String ipAddress, int port) {
  SkywireTcpClient::ipAddress = ipAddress;
  SkywireTcpClient::port = port;
  SkywireTcpClient::skywire = Skywire();
}

bool SkywireTcpClient::open() {
  skywire.start();

  return skywire.openSocketConnection(ipAddress, port);
}

bool SkywireTcpClient::send(String message) {
  return skywire.sendMessageInSocketConnection(message);;
}

bool SkywireTcpClient::close() {
  return skywire.closeSocketConnection();
}