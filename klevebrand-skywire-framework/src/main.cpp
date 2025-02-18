#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../lib/klevebrand-skywire-framework/klevebrand-skywire-tcp-client.h"

SkywireTcpClient skywireTcpClient("213.66.134.107", 31300);

void setup()
{
  Serial.begin(115200);

  skywireTcpClient.open();
}

void loop()
{
  if (skywireTcpClient.available()) {
    Serial.println(skywireTcpClient.readString());
  }
  
  if (Serial.available())
  {
    Serial.println("Serial is available!");

    skywireTcpClient.send(Serial.readString());
  }
}