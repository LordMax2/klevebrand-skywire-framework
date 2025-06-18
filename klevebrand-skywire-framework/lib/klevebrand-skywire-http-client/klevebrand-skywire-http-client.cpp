#include "klevebrand-skywire-http-client.h"

SkywireHttpClient::SkywireHttpClient(String base_url, int port)
{
    this->base_url = base_url;
    this->port = port;

    skywire = Skywire();
};

void SkywireHttpClient::get(String query)
{
    skywire.configureHttp("flightcontroltower.klevebrand.se");

    skywire.queryHttp(query);
}