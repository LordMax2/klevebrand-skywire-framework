#include "klevebrand-skywire-tcp-client.h"

SkywireTcpClient::SkywireTcpClient(HardwareSerial skywire_serial, String ip_address, int port) : skywire(skywire_serial)
{
  SkywireTcpClient::ip_address = ip_address;
  SkywireTcpClient::port = port;
}

bool SkywireTcpClient::open()
{
  skywire.start();

  socket_dial_connection_id = skywire.openTcpSocketConnection(ip_address, port);

  return socket_dial_connection_id > 0;
}

bool SkywireTcpClient::send(String message)
{
  return skywire.sendMessageInTcpSocketConnection(message, socket_dial_connection_id);
}

bool SkywireTcpClient::close()
{
  return skywire.closeTcpSocketConnection(socket_dial_connection_id);
}

int SkywireTcpClient::available()
{
  return skywire.available();
}

String SkywireTcpClient::readString()
{
  return skywire.readString();
}