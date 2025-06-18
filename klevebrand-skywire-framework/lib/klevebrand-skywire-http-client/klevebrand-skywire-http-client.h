#ifndef KLEVEBRAND_SKYWIRE_HTTP_CLIENT_H
#define KLEVEBRAND_SKYWIRE_HTTP_CLIENT_H

#include "../klevebrand-skywire-framework/klevebrand-skywire-framework.h"

class SkywireHttpClient
{
public:
    SkywireHttpClient(String base_url, int port);

    Skywire skywire;

    void get(String query);
private:
    String base_url;
    int port;
};

#endif // KLEVEBRAND_SKYWIRE_HTTP_CLIENT_H