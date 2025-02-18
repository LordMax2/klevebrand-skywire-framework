#include "klevebrand-skywire-tcp-client.h"

SkywireTcpClient::SkywireTcpClient(String ipAddress, int port) {
  SkywireTcpClient::ipAddress = ipAddress;
  SkywireTcpClient::port = port;
  SkywireTcpClient::skywire = Skywire();
}

bool SkywireTcpClient::open() {
  skywire.start();

  socketDialConnectionId = skywire.openTcpSocketConnection(ipAddress, port);

  return socketDialConnectionId > 0;
}

bool SkywireTcpClient::send(String message) {
  return skywire.sendMessageInTcpSocketConnection(message, socketDialConnectionId);
}

bool SkywireTcpClient::close() {
  return skywire.closeTcpSocketConnection(socketDialConnectionId);
}

int SkywireTcpClient::available() {
  return skywire.available();
}

String SkywireTcpClient::readString() {
  return skywire.readString();
}