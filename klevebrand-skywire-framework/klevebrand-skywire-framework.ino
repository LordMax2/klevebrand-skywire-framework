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