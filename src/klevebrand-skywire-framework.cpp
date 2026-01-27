#include "Arduino.h"
#include "klevebrand-skywire-framework.h"

static int socket_dial_connection_id_counter = 1;

void Skywire::start()
{
  _skywire_serial.begin(115200);
  Serial.println("STARTING SKYWIRE MODEM...");

  while (!sendAt())
  {
    Serial.println("NO RESPONSE FROM MODEM. RETRYING IN 1 SECOND...");
    delay(1000);
  }
  disableEcho();

  if (_debug_mode)
  {
    setVerboseOutputLevel(2);
  }

  configureHologramApn();

  waitUntilConnectedToNetwork();

  enablePacketDataProtocol();

  Serial.println("SKYWIRE MODEM STARTED.");
}

void Skywire::reboot()
{
  print("AT#ENHRST=1,0\r");

  waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);
}

int Skywire::available()
{
  return _skywire_serial.available();
}

String Skywire::readString()
{
  return _skywire_serial.readString();
}

int Skywire::read()
{
  return _skywire_serial.read();
}

size_t Skywire::print(String payload)
{
  if (_debug_mode)
    Serial.println(payload);

  size_t result = _skywire_serial.print(payload);

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

  return "PLACEHOLDER";
}

bool Skywire::setVerboseOutputLevel(int level)
{
  // CMEE 0 = disable, 1 numeric error values, 2 verbose error values
  print("AT+CMEE=" + String(level) + "\r");

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY).is_success;
}

int Skywire::openTcpSocketConnection(String ip_address, int port)
{
  socket_dial_connection_id_counter++;

  print("AT#SD= " + String(socket_dial_connection_id_counter) + ",0," + String(port) + ",\"" + ip_address + "\"\r");

  waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  print("AT#SCFG=" + String(socket_dial_connection_id_counter) + ",1,0,0,100,100\r");

  waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  return socket_dial_connection_id_counter;
}

bool Skywire::sendMessageInTcpSocketConnection(String message, int socket_dial_connection_id)
{
  print("AT#SSEND=" + String(socket_dial_connection_id) + "\r");

  print(message);

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY).is_success;
}

bool Skywire::closeTcpSocketConnection(int socket_dial_connection_id)
{
  print("AT#SH=" + String(socket_dial_connection_id) + "\r");

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

void Skywire::waitUntilConnectedToNetwork()
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

SkywireResponseResult_t Skywire::waitForSkywireResponse(unsigned long milliseconds_to_wait)
{
  long start_milliseconds = millis();
  String response_content = "";

  while (start_milliseconds + milliseconds_to_wait > millis())
  {
    if (_skywire_serial.available())
    {
      response_content += _skywire_serial.readString();
      if (_debug_mode)
        Serial.println("RECEIVED: " + response_content);

      SkywireResponseResult_t result(true, response_content);

      return result;
    }
  }

  SkywireResponseResult_t result_failure(false, "NO RESPONSE FROM MODEM");
  return result_failure;
}

bool Skywire::httpConfigureParameters(String url, int port)
{
  print("AT#HTTPCFG=0,\"" + url + "\"\r");

  SkywireResponseResult_t response = waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  return response.is_success;
}

bool Skywire::httpSendRequest(String path)
{
  print("AT#HTTPQRY=0,0,\"/" + path + "\"\r");

  SkywireResponseResult_t response = waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  return response.is_success;
}

bool Skywire::httpWaitForHttpRing(unsigned long timeout_milliseconds)
{
  long start_milliseconds = millis();

  while (true)
  {
    SkywireResponseResult_t response = waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

    if (response.response_content.indexOf("HTTPRING") != -1)
    {
      return true;
    }

    if (start_milliseconds + timeout_milliseconds < millis())
    {
      return false;
    }
  }

  return false;
}

SkywireResponseResult_t Skywire::httpReadResponse()
{
  print("AT#HTTPRCV=0\r");

  SkywireResponseResult_t response = waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  return response;
}

bool Skywire::enableGps()
{
  print("AT$GPSP=1\r");

  SkywireResponseResult_t response = waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  return response.is_success;
}

SkywireResponseResult_t Skywire::getGpsData()
{
  print("AT$GPSACP\r");

  SkywireResponseResult_t response = waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  return response;
}

bool Skywire::setGpsDataStreamingMode(bool enable_streaming)
{
  String command = "AT$GPSNMUN=";

  if (enable_streaming)
    command += "1\r";
  else
    command += "0\r";

  print(command);

  SkywireResponseResult_t response = waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

  return response.is_success;
}
