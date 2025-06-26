#include "klevebrand-skywire-http-client.h"

String SkywireHttpClient::get(String path)
{
    prepareHttpClient();
    sendHttpRequest(path);

    return readHttpRing();
}

bool SkywireHttpClient::prepareHttpClient()
{
    skywire.print("AT#HTTPCFG=0,\"" + baseUrl + "\"\r");

    return skywire.waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &responseOkSerialPrint);
}

bool SkywireHttpClient::sendHttpRequest(String path)
{
    skywire.print("AT#HTTPQRY=0,0,\"/" + path + "\"\r");

    return skywire.waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &responseIsHttpRing);
}

String SkywireHttpClient::readHttpRing()
{
    skywire.print("AT#HTTPRCV=0\r");

    skywire.waitForSkywireResponse(BASE_WAIT_FOR_RESPONSE_DELAY, &responseIsHttpRing);

    return "";
}

bool SkywireHttpClient::responseOkSerialPrint(String responseContent)
{
    Serial.println(responseContent);
    return true;
}

bool SkywireHttpClient::responseIsHttpRing(String responseContent)
{
    return responseContent.indexOf("HTTPRING") > 0;
}