#include "Arduino.h"
#include "klevebrand-skywire-framework.h"

SoftwareSerial Skywire::skywireSerialChannel(SKYWIRE_RX_PIN, SKYWIRE_TX_PIN);
static int socketDialConnectionIdCounter = 1;

void Skywire::start()
{
  Serial.println("Starting skywire modem...");

  skywireSerialChannel.begin(115200);

  sendAt();
  disableEcho();
  enableVerboseOutput();

  Serial.println("Skywire modem started.");

  configureHologramApn();

  isConnectedToNetwork();

  enablePacketDataProtocol();
}

void Skywire::stop()
{
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
  if(DEBUG)
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
  print("AT#SSEND=" + String(socketDialConnectionId));

  print(message);

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &responseOkSerialPrint);
}

bool Skywire::closeTcpSocketConnection(int socketDialConnectionId)
{
  print("AT#SH=" + String(socketDialConnectionId));

  return true;
}

void Skywire::waitUntilConnectedToHomeNetwork()
{
  while (!isConnectedToNetwork())
  {
    delay(1000);
  }
}

void Skywire::enablePacketDataProtocol()
{
  print("AT#SGACT=1,1\r");

  waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &responseOkSerialPrint);
}

bool Skywire::isConnectedToNetwork()
{
  Serial.println("Verifying network connection...");

  // Response format: +CEREG: 0,<stat> OK, verify that <stat> is either 1 or 5, otherwise failure
  print("AT+CEREG?\r");

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &isConnectedToNetworkResponseOk);
}

bool Skywire::isConnectedToNetworkResponseOk(String responseContent)
{
  // Response format: +CEREG: 0,<stat> OK, verify that <stat> is either 1 or 5, otherwise failure
  if (responseContent.indexOf(",5") != -1 || responseContent.indexOf(",1") != -1)
  {
    Serial.println("Connected to network.");
    
    return true;
  }

  return false;
}

void Skywire::configureHologramApn()
{
  Serial.println("Configuring hologram.io as APN...");

  String command = "AT+CGDCONT=1,\"IPV4V6\",\"hologram\"\r";

  print(command);

  while (!isHologramApnSuccessfullyConfigured())
  {
    Serial.println("Failed to configure hologram.io as APN. Retrying in 1 second.");

    print(command);

    delay(1000);
  }

  Serial.println("Hologram set as APN.");
}

bool Skywire::isHologramApnSuccessfullyConfigured()
{
  Serial.println("Verifying hologram APN configuration...");

  print("AT+CGDCONT?\r");

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &isHologramApnSuccessfullyConfiguredResponseOk);
}

bool Skywire::isHologramApnSuccessfullyConfiguredResponseOk(String responseContent)
{
  if (responseContent.indexOf("hologram") != -1)
  {
    Serial.println("Hologram APN is configured.");

    return true;
  }

  return false;
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
      Serial.println(responseContent);

      if (isResponseContentValid(responseContent))
      {
        return true;
      }
    }
  }

  return false;
}
