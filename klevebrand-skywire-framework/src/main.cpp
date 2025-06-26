#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../lib/klevebrand-skywire-http-client/klevebrand-skywire-http-client.h"

SkywireHttpClient httpClient("flightcontroltower.klevebrand.se", 80);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if (httpClient.skywire.available())
  {
    Serial.println(httpClient.skywire.readString());
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
      httpClient.skywire.print(string);
    }
  }
}