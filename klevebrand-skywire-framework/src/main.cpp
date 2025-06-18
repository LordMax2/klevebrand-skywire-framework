#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../lib/klevebrand-skywire-http-client/klevebrand-skywire-http-client.h"

SkywireHttpClient skywireHttpClient("flightcontroltower.klevebrand.se", 80);

void setup()
{
  Serial.begin(115200);

  skywireHttpClient.skywire.start();
}

void loop()
{
  skywireHttpClient.get("/api/v1/weather/coordinates?latitude=59.8586&longitude=17.6389");

  delay(1000);
}