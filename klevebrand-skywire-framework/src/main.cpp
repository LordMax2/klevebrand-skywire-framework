#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../lib/klevebrand-skywire-framework/klevebrand-skywire-tcp-client/klevebrand-skywire-tcp-client.h"

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
    String incoming = Serial.readString();

    int incomingLength = incoming.length();
    char buffer[incomingLength + 1];

    incoming.toCharArray(buffer, incomingLength + 1);

    skywireTcpClient.write(buffer);
  }
}