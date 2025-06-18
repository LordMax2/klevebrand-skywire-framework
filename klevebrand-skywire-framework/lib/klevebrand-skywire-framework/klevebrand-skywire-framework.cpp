#include "Arduino.h"
#include "klevebrand-skywire-framework.h"
#include "configure-hologram-command.h"
#include "connect-to-network-command.h"

SoftwareSerial Skywire::skywireSerialChannel(SKYWIRE_RX_PIN, SKYWIRE_TX_PIN);
static int socketDialConnectionIdCounter = 1;

void Skywire::start()
{
  Serial.println("Starting skywire modem...");

  skywireSerialChannel.begin(115200);

  delay(500);

  sendAt();
  disableEcho();
  enableVerboseOutput();

  Serial.println("Skywire modem started.");

  ConfigureHologramCommand configureHologramCommand;
  configureHologramCommand.Execute(this); 

  ConnectToNetworkCommand connectToNetworkCommand;
  connectToNetworkCommand.Execute(this);

  enablePacketDataProtocol();
}

void Skywire::reboot()
{
  print("AT#ENHRST=1,0\r");
  waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &responseOkSerialPrint);
}

int Skywire::available()
{
  return skywireSerialChannel.available();
}

String Skywire::readString()
{
  return skywireSerialChannel.readString();
}

size_t Skywire::print(String payload)
{
  if (DEBUG)
    Serial.println(payload);

  size_t result = skywireSerialChannel.print(payload);

  delay(BASE_DELAY);

  return result;
}

void Skywire::sendAt()
{
  print("AT\r");
}

void Skywire::disableEcho()
{
  print("ATE0\r");
}

String Skywire::getConnectedNetworkProviderName()
{
  Serial.println("Retriving conencted network provider name...");

  // Response format: +COPS: <mode>[,<format>[,<oper>][,<Act>]]
  print("AT+COPS?\r");
}

void Skywire::enableVerboseOutput()
{
  // CMEE 0 = disable, 1 numeric error values, 2 verbose error values
  print("AT+CMEE=2\r");
}

int Skywire::openTcpSocketConnection(String ipAddress, int port)
{
  socketDialConnectionIdCounter++;

  print("AT#SD= " + String(socketDialConnectionIdCounter) + ",0," + String(port) + ",\"" + ipAddress + "\"\r");

  waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &isOpenTcpSocketConnectionResponseOk);

  print("AT#SCFG=" + String(socketDialConnectionIdCounter) + ",1,0,0,100,100\r");

  waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &responseOkSerialPrint);

  return socketDialConnectionIdCounter;
}

bool Skywire::isOpenTcpSocketConnectionResponseOk(String responseContent)
{
  if (responseContent.indexOf("CONNECT") != -1)
  {
    return true;
  }

  Serial.println("Failed to connect to socket, message: " + responseContent);

  return false;
}

bool Skywire::sendMessageInTcpSocketConnection(String message, int socketDialConnectionId)
{
  print("AT#SSEND=" + String(socketDialConnectionId) + "\r");

  print(message);

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &responseOkSerialPrint);
}

bool Skywire::closeTcpSocketConnection(int socketDialConnectionId)
{
  print("AT#SH=" + String(socketDialConnectionId) + "\r");

  return true;
}

bool Skywire::closeAllTcpSocketConnection()
{
  for (int i = 0; i < 6; i++)
  {
    print("AT#SH=" + String(i) + "\r");
    waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &responseOkSerialPrint);
  }

  return true;
}

void Skywire::enablePacketDataProtocol()
{
  print("AT#SGACT=1,1\r");

  waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &responseOkSerialPrint);
}

void Skywire::disablePacketDataProtocol()
{
  print("AT#SGACT=1,0\r");

  waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &responseOkSerialPrint);
}

bool Skywire::configureHttp(String base_url)
{
  Serial.println("Setting " + base_url + "as the HTTP base url...");

  print("AT#HTTPCFG=0,\"" + String(base_url) + "\"\r");

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, responseOkSerialPrint);
}

String Skywire::queryHttp(String query)
{
  Serial.println("Querying http...");

  print("AT#HTTPQRY=0,0," + query + "\"\r");

 waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, responseOkSerialPrint);

  return "return just nu temp";
}

bool Skywire::responseOkSerialPrint(String responseContent)
{
  Serial.println(responseContent);
  return true;
}

bool Skywire::waitForSkywireResponse(int millisecondsToWait, bool (*isResponseContentValid)(String))
{
  long startMillis = millis();
  String responseContent = "";

  while (startMillis + millisecondsToWait > millis())
  {
    if (skywireSerialChannel.available())
    {
      responseContent += skywireSerialChannel.readString();
      if (DEBUG)
        Serial.println("Received: " + responseContent);

      if (isResponseContentValid(responseContent))
      {
        return true;
      }
    }
  }

  return false;
}
