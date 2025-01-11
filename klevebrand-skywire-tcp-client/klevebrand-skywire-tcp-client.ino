/*
* This is a test sketch for development with a Skywire modem using an Arduino and the base Skywire framework abstraction.
*/

#include "klevebrand-skywire-tcp-client.h"

SkywireTcpClient skywireTcpClient("213.66.134.107", 31300);

void setup() {
  Serial.begin(115200);

  skywireTcpClient.open();
}

void loop() {
  if (skywireTcpClient.skywire.skywireSerialChannel.available())
    Serial.println(skywireTcpClient.skywire.skywireSerialChannel.readString());
  if (Serial.available())
    skywireTcpClient.skywire.skywireSerialChannel.write(Serial.read()); 
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