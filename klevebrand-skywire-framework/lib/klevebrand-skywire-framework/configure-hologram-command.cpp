#include "configure-hologram-command.h"
#include "Arduino.h"

bool ConfigureHologramCommand::Execute(Skywire *skywire)
{
  Serial.println("Configuring hologram.io as APN...");

  if (isHologramApnSuccessfullyConfigured(skywire))
  {
    Serial.println("Hologram is already configured.");

    return true;
  }

  String command = "AT+CGDCONT=1,\"IPV4V6\",\"hologram\"\r";

  while (!isHologramApnSuccessfullyConfigured(skywire))
  {
    skywire->print(command);

    delay(50);
  }

  skywire->print(command);

  Serial.println("Hologram set as APN.");

  return true;
}

bool ConfigureHologramCommand::isHologramApnSuccessfullyConfigured(Skywire *skywire)
{
  Serial.println("Verifying hologram APN configuration...");

  skywire->print("AT+CGDCONT?\r");

  return skywire->waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &isHologramApnSuccessfullyConfiguredResponseOk);
}

bool ConfigureHologramCommand::isHologramApnSuccessfullyConfiguredResponseOk(String responseContent)
{
  if (responseContent.indexOf("hologram") != -1 || responseContent.indexOf("context already activated") != -1)
  {
    Serial.println("Hologram APN is configured.");

    return true;
  }

  return false;
}