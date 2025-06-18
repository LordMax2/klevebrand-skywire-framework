#include "connect-to-network-command.h"
#include "Arduino.h"

bool ConnectToNetworkCommand::Execute(Skywire* skywire)
{
  Serial.println("Connecting to network...");

  ValidateResponse(skywire);

  Serial.println("Connected to network.");

  return true;
}

bool ConnectToNetworkCommand::ValidateResponse(Skywire* skywire)
{  
  while (!isConnectedToNetwork(skywire))
  {
    Serial.println("Not connected to network yet. Retrying in 1 second.");

    delay(1000);
  }

  return true; 
}

bool ConnectToNetworkCommand::isConnectedToNetwork(Skywire* skywire)
{
  Serial.println("Verifying network connection...");

  skywire->print("AT+CEREG?\r");

  return skywire->waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &isConnectedToNetworkResponseOk);
}

bool ConnectToNetworkCommand::isConnectedToNetworkResponseOk(String responseContent)
{
  // Response format: +CEREG: 0,<stat> OK, verify that <stat> is either 1 or 5, otherwise failure
  if (responseContent.indexOf(",5") != -1 || responseContent.indexOf(",1") != -1)
  {
    Serial.println("Connected to network.");

    return true;
  }

  return false;
}