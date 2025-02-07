/*
* This is a test sketch for development with a Skywire modem using an Arduino and the base Skywire framework abstraction.
*/

#include "klevebrand-skywire-tcp-client.h"

SkywireTcpClient skywireTcpClient("x.x.x.x", 31300);

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
