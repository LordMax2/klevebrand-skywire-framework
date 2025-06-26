#ifndef KLEVEBRAND_SKYWIRE_HTTP_CLIENT_H
#define KLEVEBRAND_SKYWIRE_HTTP_CLIENT_H

#include "Arduino.h"
#include "klevebrand-skywire-framework.h"

class SkywireHttpClient
{
public:
    SkywireHttpClient(String baseUrl, int port)
    {
        this->baseUrl = baseUrl;
        this->port = port;
    }

    String get(String path);

private:
    String baseUrl;
    int port;
    Skywire skywire;
    bool prepareHttpClient();
    bool sendHttpRequest(String path);
    String readHttpRing();
    static bool responseOkSerialPrint(String responseContent);
    static bool responseIsHttpRing(String responseContent);

};

#endif // KLEVEBRAND_SKYWIRE_HTTP_CLIENT_H