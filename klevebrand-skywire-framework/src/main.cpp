#include <Arduino.h>
//#include "../lib/klevebrand-skywire-http-client/klevebrand-skywire-http-client.h"
#include "../lib/klevebrand-skywire-http-stepper-client/klevebrand-skywire-http-stepper-client.h"


//SkywireHttpClient httpClient("flightcontroltower.klevebrand.se", 80);

SkywireHttpStepperClient stepperClient("flightcontroltower.klevebrand.se", 80);

void setup()
{
	Serial.begin(115200);

	stepperClient.start();
}

void loop()
{
	long start_micros = micros();

	stepperClient.get("api/v1/drone/1337");

	Serial.println(micros() - start_micros);

	delay(10);

	/*
	if (Serial.available())
	{
		String string = Serial.readString();

		if (string == "http\r")
		{
			String json = httpClient.get("api/v1/weather/coordinates?latitude=59.8586&longitude=17.6389");

			Serial.println(json);
		}
		else
		{
			httpClient.skywire.print(string);
		}
	}
	*/
}
