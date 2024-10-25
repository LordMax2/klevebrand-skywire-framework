#include <SoftwareSerial.h>
#include "klevebrand-skywire-framework.h"

Skywire skywire;

void setup() {
  Serial.begin(115200);

  skywire.start();

  skywire.getAssignment();
}

void loop() {
  
  skywire.getAssignment();

  delay(1000);

  //if (skywire.skywireSerialChannel.available())
  //  Serial.write(skywire.skywireSerialChannel.read());
  //if (Serial.available())
  //  skywire.skywireSerialChannel.write(Serial.read()); 
}
