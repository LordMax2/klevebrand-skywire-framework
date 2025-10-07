#ifndef KLEVEBRAND_SKYWIRE_HTTP_STEPPER_CLIENT_H
#define KLEVEBRAND_SKYWIRE_HTTP_STEPPER_CLIENT_H

#include "Arduino.h"
#include "../klevebrand-skywire-framework/klevebrand-skywire-framework.h"

class SkywireHttpStepperClient
{
public:
	SkywireHttpStepperClient(HardwareSerial skywire_serial, String base_url, int port) : skywire(skywire_serial)
	{
		this->base_url = base_url;
		this->port = port;

		resetState();
	}
	bool start();
	SkywireResponseResult_t get(String path);

private:
	Skywire skywire;
	String base_url;
	int port;

	bool httpCfg();
	bool http_cfg_sent = false;
	bool http_cfg_ok_recieved = false;

	bool httpQry(String path);
	bool http_qry_sent = false;
	bool http_qry_ok_recieved = false;

	bool httpRcv();
	bool http_rcv_sent = false;
	bool http_rcv_ok_recieved = false;

	bool httpRing();
	bool http_ring_recieved = false;

	void resetState();
	void serialReadToRxBuffer();

	void resetRxBuffer();
	String rx_buffer;
};

#endif // KLEVEBRAND_SKYWIRE_HTTP_STEPPER_CLIENT_H
