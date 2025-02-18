#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../lib/klevebrand-skywire-framework/klevebrand-skywire-tcp-client.h"

SkywireTcpClient skywireTcpClient("XXX.XXX.XXX.XXX", 0);

void setup()
{
  Serial.begin(115200);

  skywireTcpClient.open();
}

void loop()
{
  if (skywireTcpClient.available())
  {
    Serial.println(skywireTcpClient.readString());
  }

  if (Serial.available())
  {
    Serial.println("Serial is available!");

    skywireTcpClient.send(Serial.readString());
  }
}