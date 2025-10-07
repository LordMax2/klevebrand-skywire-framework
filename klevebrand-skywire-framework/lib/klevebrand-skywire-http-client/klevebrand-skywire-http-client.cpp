#include "klevebrand-skywire-http-client.h"

String SkywireHttpClient::get(String path)
{
    skywire.start();

    prepareHttpClient();
    sendHttpRequest(path);

    bool http_ring_recieved = false;

    do
    {
        SkywireResponseResult_t result = skywire.waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

        if (result.response_content.indexOf("HTTPRING") > 0)
        {
            http_ring_recieved = true;
        }
    } while (!http_ring_recieved);

    return readHttpRing();
}

bool SkywireHttpClient::prepareHttpClient()
{
    skywire.print("AT#HTTPCFG=0,\"" + _base_url + "\"\r");

    return skywire.waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY).is_success;
}

bool SkywireHttpClient::sendHttpRequest(String path)
{
    skywire.print("AT#HTTPQRY=0,0,\"/" + path + "\"\r");

    SkywireResponseResult_t result = skywire.waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

    return result.is_success;
}

String SkywireHttpClient::readHttpRing()
{
    skywire.print("AT#HTTPRCV=0\r");

    SkywireResponseResult_t result = skywire.waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY);

    return result.response_content;
}
