#include "klevebrand-skywire-tcp-client.h"

SkywireTcpClient::SkywireTcpClient(String ipAddress, int port) {
  SkywireTcpClient::ipAddress = ipAddress;
  SkywireTcpClient::port = port;
  SkywireTcpClient::skywire = Skywire();
}

bool SkywireTcpClient::open() {
  skywire.start();

  return skywire.openTcpSocketConnection(ipAddress, port);
}

bool SkywireTcpClient::send(String message) {
  return skywire.sendMessageInTcpSocketConnection(message);;
}

bool SkywireTcpClient::close() {
  return skywire.closeTcpSocketConnection();
}

int SkywireTcpClient::available() {
  return skywire.available();
}

String SkywireTcpClient::readString() {
  return skywire.readString();
}

size_t SkywireTcpClient::write(char *payload) {
  return skywire.write(payload);
}