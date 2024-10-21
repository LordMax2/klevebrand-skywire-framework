#include "Arduino.h"
#include "klevebrand-skywire-framework.h"

SoftwareSerial Skywire::skywireSerialChannel(SKYWIRE_RX_PIN, SKYWIRE_TX_PIN);

void Skywire::start() {
    Serial.println("Starting skywire modem...");

    skywireSerialChannel.begin(115200);  
    delay(1000);
    skywireSerialChannel.println("ATE0\r");
    delay(1000);
    skywireSerialChannel.println("AT\r");
    delay(1000);

    Serial.println("Skywire modem started.");

    configureHologramApn();

    

  if(!isHologramApnSuccessfullyConfigured()) {
    Serial.println("Failed to configure hologram.io as APN. Program stopped.");

    while (true) {}
  }
}

bool Skywire::isHologramApnSuccessfullyConfigured() {
  Serial.println("Verifying hologram APN configuration...");

  /* Some kind of verification here
  delay(10000);

  skywireSerialChannel.println("AT\r");

  delay(250);

  skywireSerialChannel.println("AT+CGDCONT?\r");
  
  long startMillis = millis();

  while(startMillis + 10000 > millis()) {
    if(skywireSerialChannel.available()) {
      Serial.println(skywireSerialChannel.read());
    }
  }

  Serial.println("Finished verifying hologram APN configuration.");
  */

  return true;
}

void Skywire::configureHologramApn() {
  Serial.println("Configuring hologram.io as APN...");

  Skywire::skywireSerialChannel.println("AT+CGDCONT=4,\"IPV4V6\",\"hologram\"\r");

  delay(1000);

  Serial.println("Hologram set as APN.");
}

void Skywire::stop() {
    
}
