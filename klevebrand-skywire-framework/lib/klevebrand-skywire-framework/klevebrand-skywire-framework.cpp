#include "Arduino.h"
#include "klevebrand-skywire-framework.h"

static int socketDialConnectionIdCounter = 1;

void Skywire::start()
{
  Serial3.begin(115200);
  Serial.println("STARTING SKYWIRE MODEM...");

  sendAt();
  disableEcho();
  enableVerboseOutput();

  configureHologramApn();

  isConnectedToNetwork();

  enablePacketDataProtocol();

  Serial.println("SKYWIRE MODEM STARTEd.");
}

void Skywire::reboot()
{
  print("AT#ENHRST=1,0\r");
  waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);
}

int Skywire::available()
{
  return Serial3.available();
}

String Skywire::readString()
{
  return Serial3.readString();
}

size_t Skywire::print(String payload)
{
  if (DEBUG)
    Serial.println(payload);

  size_t result = Serial3.print(payload);

  delay(BASE_DELAY);

  return result;
}

bool Skywire::sendAt()
{
  print("AT\r");

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY).is_success;
}

bool Skywire::disableEcho()
{
  print("ATE0\r");

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY).is_success;
}

String Skywire::getConnectedNetworkProviderName()
{
  Serial.println("RETRIVING CONENCTED NETWORK PROVIDER NAME...");

  // Response format: +COPS: <mode>[,<format>[,<oper>][,<Act>]]
  print("AT+COPS?\r");
}

bool Skywire::enableVerboseOutput()
{
  // CMEE 0 = disable, 1 numeric error values, 2 verbose error values
  print("AT+CMEE=2\r");

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY).is_success;
}

int Skywire::openTcpSocketConnection(String ipAddress, int port)
{
  socketDialConnectionIdCounter++;

  print("AT#SD= " + String(socketDialConnectionIdCounter) + ",0," + String(port) + ",\"" + ipAddress + "\"\r");

  waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  print("AT#SCFG=" + String(socketDialConnectionIdCounter) + ",1,0,0,100,100\r");

  waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  return socketDialConnectionIdCounter;
}

bool Skywire::sendMessageInTcpSocketConnection(String message, int socketDialConnectionId)
{
  print("AT#SSEND=" + String(socketDialConnectionId) + "\r");

  print(message);

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY).is_success;
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
    waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);
  }

  return true;
}

void Skywire::waitUntilConnectedToHomeNetwork()
{
  while (!isConnectedToNetwork())
  {
    delay(1000);
  }
}

bool Skywire::enablePacketDataProtocol()
{
  print("AT#SGACT=1,1\r");

  SkywireResponseResult_t result = waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  return result.is_success;
}

bool Skywire::disablePacketDataProtocol()
{
  print("AT#SGACT=1,0\r");

  SkywireResponseResult_t result = waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  return result.is_success;
}

bool Skywire::isConnectedToNetwork()
{
  Serial.println("VERIFYING NETWORK CONNECTION...");

  print("AT+CEREG?\r");

  SkywireResponseResult_t result = waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  // Response format: +CEREG: 0,<stat> OK, verify that <stat> is either 1 or 5, otherwise failure
  if (result.response_content.indexOf(",5") != -1 || result.response_content.indexOf(",1") != -1)
  {
    Serial.println("CONNECTED TO NETWORK.");

    return true;
  }

  Serial.println("NOT TO CONNECT TO NETWORK.");
  return false;
}

bool Skywire::configureHologramApn()
{
  Serial.println("CONFIGURING HOLOGRAM.IO AS APN...");

  String command = "AT+CGDCONT=1,\"IPV4V6\",\"hologram\"\r";

  print(command);

  while (!isHologramApnSuccessfullyConfigured())
  {
    Serial.println("FAILED TO CONFIGURE HOLOGRAM.IO AS APN. RETRYING IN 1 SECOND.");

    print(command);

    delay(1000);
  }

  Serial.println("HOLOGRAM SET AS APN.");

  return true;
}

bool Skywire::isHologramApnSuccessfullyConfigured()
{
  Serial.println("VERIFYING HOLOGRAM APN CONFIGURATION...");

  print("AT+CGDCONT?\r");

  SkywireResponseResult_t result = waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  if (result.response_content.indexOf("hologram") != -1 || result.response_content.indexOf("context already activated") != -1)
  {
    Serial.println("HOLOGRAM apn IS CONFIGURED.");

    return true;
  }

  return false;
}

SkywireResponseResult_t Skywire::waitForSkywireResponse(int millisecondsToWait)
{
  long startMillis = millis();
  String responseContent = "";

  while (startMillis + millisecondsToWait > millis())
  {
    if (Serial3.available())
    {
      responseContent += Serial3.readString();
      if (DEBUG)
        Serial.println("RECEIVED: " + responseContent);

      SkywireResponseResult_t result(true, responseContent);

      return result;
    }
  }

  SkywireResponseResult_t result_failure(false, "NO RESPONSE FROM MODEM");
  return result_failure;
}
