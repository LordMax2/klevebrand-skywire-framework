#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../lib/klevebrand-skywire-framework/klevebrand-skywire-framework.h"
#include "../lib/klevebrand-skywire-framework/klevebrand-skywire-http-client.h"

Skywire skywire;
SkywireHttpClient httpClient("flightcontroltower.klevebrand.se", 80);

void setup()
{
  Serial.begin(115200);

  skywire.start();
}

void loop()
{
  if (skywire.available())
  {
    Serial.println(skywire.readString());
  }

  if (Serial.available())
  {
    String string = Serial.readString();

    if (string == "http\r")
    {
      httpClient.get("api/v1/weather/coordinates?latitude=59.8586&longitude=17.6389");
    }
    else
    {
      skywire.print(string);
    }
  }
}