#include "Arduino.h"
#include "klevebrand-skywire-framework.h"

SoftwareSerial Skywire::skywireSerialChannel(SKYWIRE_RX_PIN, SKYWIRE_TX_PIN);

void Skywire::start() {
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

void Skywire::stop() {
}

void Skywire::sendAt() {
  skywireSerialChannel.println("AT\r");
  delay(BASE_DELAY);
}

void Skywire::disableEcho() {
  skywireSerialChannel.println("ATE0\r");
  delay(BASE_DELAY);
}

String Skywire::getConnectedNetworkProviderName() {
  Serial.println("Retriving conencted network provider name...");

  // Response format: +COPS: <mode>[,<format>[,<oper>][,<Act>]]
  skywireSerialChannel.println("AT+COPS?\r");
  delay(BASE_DELAY);
}

void Skywire::enableVerboseOutput() {
  skywireSerialChannel.println("AT+CMEE=2\r");
  delay(BASE_DELAY);
}

bool Skywire::openTcpSocketConnection(String ipAddress, int port) {
  skywireSerialChannel.println("AT#SD=1,0," + String(port) + ",\"" + ipAddress + "\"\r");

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &isOpenTcpSocketConnectionResponseOk);
}

bool Skywire::isOpenTcpSocketConnectionResponseOk(String responseContent) {
  if (responseContent.indexOf("CONNECT") != -1) {
    return true;
  }

  Serial.println("Failed to connect to socket, message: " + responseContent);

  return false;
}

bool Skywire::sendMessageInTcpSocketConnection(String message) {
  skywireSerialChannel.println("AT#SSEND=1");

  delay(BASE_DELAY);

  skywireSerialChannel.println(message);

  delay(BASE_DELAY);
  
  skywireSerialChannel.println("AT#SRECV=1,1500");

  return waitForSkywireResponse(5000, &responseOkSerialPrint);
}

bool Skywire::closeTcpSocketConnection() {
  skywireSerialChannel.println("AT#SH=1");

  return true;
}

void Skywire::waitUntilConnectedToHomeNetwork() {
  while (!isConnectedToNetwork()) {
    delay(1000);
  }
}

void Skywire::enablePacketDataProtocol() {
  skywireSerialChannel.println("AT#SGACT=1,1\r");
  delay(BASE_DELAY);
}

bool Skywire::isConnectedToNetwork() {
  Serial.println("Verifying network connection...");

  // Response format: +CEREG: 0,<stat> OK, verify that <stat> is either 1 or 5, otherwise failure
  skywireSerialChannel.println("AT+CEREG?\r");

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &isConnectedToNetworkResponseOk);
}

bool Skywire::isConnectedToNetworkResponseOk(String responseContent) {
  // Response format: +CEREG: 0,<stat> OK, verify that <stat> is either 1 or 5, otherwise failure
  if (responseContent.indexOf(",5") != -1 || responseContent.indexOf(",1") != -1) {
    Serial.println("Connected to network.");

    delay(BASE_DELAY);

    return true;
  }

  return false;
}

void Skywire::configureHologramApn() {
  Serial.println("Configuring hologram.io as APN...");

  skywireSerialChannel.println("AT+CGDCONT=1,\"IPV4V6\",\"hologram\"\r");

  while (!isHologramApnSuccessfullyConfigured()) {
    Serial.println("Failed to configure hologram.io as APN. Retrying in 1 second.");

    delay(1000);
  }

  Serial.println("Hologram set as APN.");
  delay(BASE_DELAY);
}

bool Skywire::isHologramApnSuccessfullyConfigured() {
  Serial.println("Verifying hologram APN configuration...");

  skywireSerialChannel.println("AT+CGDCONT?\r");

  return waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &isHologramApnSuccessfullyConfiguredResponseOk);
}

bool Skywire::isHologramApnSuccessfullyConfiguredResponseOk(String responseContent) {
  if (responseContent.indexOf("hologram") != -1) {
    Serial.println("Hologram APN is configured.");

    return true;
  }

  return false;
}

bool Skywire::responseOkSerialPrint(String responseContent) {
  Serial.println(responseContent);
  return true;
}

bool Skywire::waitForSkywireResponse(int millisecondsToWait, bool (*isResponseContentValid)(String)) {
  long startMillis = millis();
  String responseContent = "";

  while (startMillis + millisecondsToWait > millis()) {
    if (skywireSerialChannel.available()) {
      responseContent += skywireSerialChannel.readString();

      if (isResponseContentValid(responseContent)) {
        return true;
      }
    }
  }

  return false;
}
