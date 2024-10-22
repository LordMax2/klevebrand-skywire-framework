#include "Arduino.h"
#include "klevebrand-skywire-framework.h"

SoftwareSerial Skywire::skywireSerialChannel(SKYWIRE_RX_PIN, SKYWIRE_TX_PIN);

void Skywire::start() {
    Serial.println("Starting skywire modem...");

    skywireSerialChannel.begin(115200); 

    sendAT(); // Just to ping the device and make sure it is awake
    disableEcho();

    Serial.println("Skywire modem started.");

    configureHologramApn();
}

void Skywire::stop() {
    
}

String Skywire::getConnectedNetworkProviderName() {
  Serial.println("Retriving conencted network provider name...");

  skywireSerialChannel.println("AT+COPS?\r");

  // Output format: +COPS: <mode>[,<format>[,<oper>][,<Act>]]
}

bool Skywire::isConnectedToNetwork() {
  Serial.println("Verifying network connection...");

  skywireSerialChannel.println("AT+CEREG\r");

  // Output format: +CEREG: 0,<stat> OK, verify that <stat> is either 1 or 5, otherwise failure
}

bool Skywire::isHologramApnSuccessfullyConfigured() {
  Serial.println("Verifying hologram APN configuration...");
  
  skywireSerialChannel.println("AT+CGDCONT?\r");
  delay(200);

  // Read the output here and make sure there is a record for hologram

  return true;
}

String Skywire::getSignalStrengthAndQuality() {
  Serial.println("Retriving signal strength and quality...");

  skywireSerialChannel.println("AT+CSQ\r");
  delay(200);

  // Output format: +CSQ: <x>,<y>, where <x> is bit error rate, and <y> is signal strength
}

String Skywire::getIpAddress() {
  Serial.println("Retriving ip address...");

  skywireSerialChannel.println("AT#SGACT=1,1\r");
  delay(200);

  // Output format: #SGACT: <IPV4>,<IPV6>
}

void Skywire::configureHologramApn() {
  Serial.println("Configuring hologram.io as APN...");

  skywireSerialChannel.println("AT+CGDCONT=4,\"IPV4V6\",\"hologram\"\r");

  delay(200);

  if(!isHologramApnSuccessfullyConfigured()) {
    Serial.println("Failed to configure hologram.io as APN. Program stopped.");

    while (true) {}
  }

  Serial.println("Hologram set as APN.");
}

void Skywire::sendAT() {
  skywireSerialChannel.println("AT\r");
  delay(200);
}

void Skywire::disableEcho() {
  skywireSerialChannel.println("ATE0\r");
  delay(200);
}
