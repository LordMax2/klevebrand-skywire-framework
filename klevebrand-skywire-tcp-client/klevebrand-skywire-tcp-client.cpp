#include "klevebrand-skywire-tcp-client.h"
#include "klevebrand-skywire-framework.h"

SkywireTcpClient::SkywireTcpClient(String ipAddress, int port) {
  SkywireTcpClient::ipAddress = ipAddress;
  SkywireTcpClient::port = port;
  SkywireTcpClient::skywire = Skywire();
}

void SkywireTcpClient::open() {
  skywire.start();

  //skywire.openSocketConnection(ipAddress, port);
}

void SkywireTcpClient::send(String payload) {

}

void SkywireTcpClient::close() {
  
}