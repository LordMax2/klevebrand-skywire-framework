#include "klevebrand-skywire-tcp-client.h"

SkywireTcpClient skywireTcpClient("x.x.x.x", 13000);

void setup() {
  Serial.begin(115200);

  skywireTcpClient.open();
}

void loop() {
  delay(1000);

  Serial.println("loop");
}
