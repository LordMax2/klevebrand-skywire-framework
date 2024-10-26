#include <SoftwareSerial.h>
#include "klevebrand-skywire-framework.h"

Skywire skywire;

void setup() {
  Serial.begin(115200);

  skywire.start();
}

void loop() {
  if (skywire.skywireSerialChannel.available())
    Serial.write(skywire.skywireSerialChannel.read());
  if (Serial.available())
    skywire.skywireSerialChannel.write(Serial.read()); 
}




/*
old test code implementation of open tcp connection to klevebrand flight control tower, dont want to remove, but neither in implementation

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
*/