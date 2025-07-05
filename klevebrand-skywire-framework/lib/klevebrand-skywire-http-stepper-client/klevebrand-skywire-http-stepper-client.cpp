#include "klevebrand-skywire-http-stepper-client.h"

void SkywireHttpStepperClient::resetState()
{
	http_cfg_ok_recieved = false;
	http_cfg_sent = false;
	http_qry_ok_recieved = false;
	http_qry_sent = false;
	http_ring_recieved = false;
	http_rcv_sent = false;

	resetRxBuffer();
}

void SkywireHttpStepperClient::resetRxBuffer()
{
	rx_buffer = "";
}

bool SkywireHttpStepperClient::start()
{
	skywire.start();
	return true;
}

SkywireResponseResult_t SkywireHttpStepperClient::get(String path)
{
	if(!httpCfg()) {
		return SkywireResponseResult_t(false, "");
	} 

	if(!httpQry(path)) {
		return SkywireResponseResult_t(false, "");
	}

	if(!httpRing()) {
		return SkywireResponseResult_t(false ,"");

	}

	if(!httpRcv()) {
		return SkywireResponseResult_t(false, "");
	}


	serialReadToRxBuffer();

	if(DEBUG) {
		Serial.println(rx_buffer);
	}

	resetState();

	return SkywireResponseResult_t(true, rx_buffer);
}

bool SkywireHttpStepperClient::httpCfg()
{
	if (!http_cfg_sent)
	{
		skywire.print("AT#HTTPCFG=0,\"" + base_url + "\"\r");

		http_cfg_sent = true;

		return false;
	}

	serialReadToRxBuffer();

	if (http_cfg_sent && !http_cfg_ok_recieved)
	{
		http_cfg_ok_recieved = rx_buffer.indexOf("OK") >= 0;

		if (http_cfg_ok_recieved)
		{
			if(DEBUG) {
				Serial.println("STEPPER CLIENT RECEIVED HTTPCFG OK");
			}

			return true;
		}

		return false;
	}

	return true;
}

bool SkywireHttpStepperClient::httpQry(String path)
{
	if (!http_qry_sent)
	{
		if(DEBUG) {
			Serial.println("Sending QRY");
		}

		http_qry_sent = skywire.print("AT#HTTPQRY=0,0,\"/" + path + "\"\r");

		return false;
	}

	serialReadToRxBuffer();

	if (!http_qry_ok_recieved)
	{
		http_qry_ok_recieved = rx_buffer.indexOf("OK\r") >= 0;

		if (http_qry_ok_recieved)
		{
			if(DEBUG) {
				Serial.println("STEPPER CLIENT RECEIVED HTTPQRY OK");
			}

			return true;
		}

		return false;
	}

	return true;
}

bool SkywireHttpStepperClient::httpRing()
{
	serialReadToRxBuffer();

	if (!http_ring_recieved)
	{
		
		http_ring_recieved = rx_buffer.indexOf("HTTPRING") > 0;

		if (http_ring_recieved)
		{
			if(DEBUG) {
				Serial.println(rx_buffer);
				Serial.println("STEPPER CLIENT RECEIVED HTTPRING OK");
			}

			return true;
		}

		return false;
	}

	return true;
}

bool SkywireHttpStepperClient::httpRcv()
{
	if (!http_rcv_sent)
	{
		http_rcv_sent = skywire.print("AT#HTTPRCV=0\r");

		return false;
	}

	return true;
}

void SkywireHttpStepperClient::serialReadToRxBuffer()
{
	while (Serial3.available())
	{
		char c = Serial3.read();
		rx_buffer += c;
	}
}
