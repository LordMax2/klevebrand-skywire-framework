#ifndef KLEVEBRAND_SKYWIRE_HTTP_CLIENT_H
#define KLEVEBRAND_SKYWIRE_HTTP_CLIENT_H

#include "Arduino.h"
#include "../klevebrand-skywire-framework/klevebrand-skywire-framework.h"

class SkywireHttpClient
{
public:
    SkywireHttpClient(HardwareSerial& skywire_serial, String base_url, int port) : skywire(skywire_serial)
    {
        this->_base_url = base_url;
        this->_port = port;
    }

    String get(String path);
    Skywire skywire;

private:
    String _base_url;
    int _port;
    bool prepareHttpClient();
    bool sendHttpRequest(String path);
    String readHttpRing();
    static bool responseOkSerialPrint(String response_content);
    static bool responseIsHttpRing(String response_content);
};

#endif // KLEVEBRAND_SKYWIRE_HTTP_CLIENT_H
