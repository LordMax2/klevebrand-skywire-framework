#include "Arduino.h"
#include "klevebrand-skywire-framework.h"

SoftwareSerial Skywire::skywireSerialChannel(SKYWIRE_RX_PIN, SKYWIRE_TX_PIN);

void Skywire::start() {
  Serial.println("Starting skywire modem...");

  skywireSerialChannel.begin(115200);

  sendAT();  // Just to ping the device and make sure it is awake
  disableEcho();
  enableVerboseOutput();

  Serial.println("Skywire modem started.");

  configureHologramApn();

  isConnectedToNetwork();

  enablePacketDataProtocol();
}

void Skywire::stop() {
}

void Skywire::getAssignment() {
  // Open TCP connection
  skywireSerialChannel.println("AT#SD=1,0,31300,\"213.66.134.107\"\r");
  delay(BASE_DELAY);

  while (skywireSerialChannel.available()) {
    String response = skywireSerialChannel.readStringUntil('\n');
    Serial.println(response);
    if (response.indexOf("CONNECT") != -1) {
      break;
    }
  }

  skywireSerialChannel.println("AT#SSEND=1");
  delay(BASE_DELAY);
  skywireSerialChannel.print("message");
  skywireSerialChannel.write(0x1A);

  delay(BASE_DELAY);
  skywireSerialChannel.println("AT#SRECV=1,1500");

  long startMillis = millis();
  String response = "";

  while (startMillis + (5 * 1000) > millis()) {
    if (skywireSerialChannel.available()) {
      String response = skywireSerialChannel.readString();
      Serial.println(response);
      Serial.println(millis() - startMillis);

      skywireSerialChannel.println("AT#SH=1");
      return;
    }
  }

  skywireSerialChannel.println("AT#SH=1");
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

  long startMillis = millis();
  String response = "";

  while (startMillis + (5 * 1000) > millis()) {
    if (skywireSerialChannel.available()) {
      response = skywireSerialChannel.readStringUntil('\n');

      if (response.indexOf(",5") != -1 || response.indexOf(",1") != -1) {
        Serial.println("Connected to network.");

        delay(BASE_DELAY);

        return true;
      }
    }
  }

  delay(BASE_DELAY);

  return false;
}

bool Skywire::isHologramApnSuccessfullyConfigured() {
  Serial.println("Verifying hologram APN configuration...");

  long startMillis = millis();
  String response = "";

  skywireSerialChannel.println("AT+CGDCONT?\r");

  while (startMillis + (5 * 1000) > millis()) {
    if (skywireSerialChannel.available()) {
      response = skywireSerialChannel.readStringUntil('\n');

      if (response.indexOf("hologram") != -1) {
        Serial.println("Hologram APN is configured.");

        delay(BASE_DELAY);

        return true;
      }
    }
  }
  delay(BASE_DELAY);

  return false;
}


String Skywire::getSignalStrengthAndQuality() {
  Serial.println("Retriving signal strength and quality...");

  // Response format: +CSQ: <x>,<y>, where <x> is bit error rate, and <y> is signal strength
  skywireSerialChannel.println("AT+CSQ\r");
  delay(BASE_DELAY);
}

String Skywire::getIpAddress() {
  Serial.println("Retriving ip address...");

  // Response format: #SGACT: <IPV4>,<IPV6>
  skywireSerialChannel.println("AT#SGACT=1,1\r");
  delay(BASE_DELAY);
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

void Skywire::sendAT() {
  skywireSerialChannel.println("AT\r");
  delay(BASE_DELAY);
}

void Skywire::disableEcho() {
  skywireSerialChannel.println("ATE0\r");
  delay(BASE_DELAY);
}
