#ifndef CONNECT_TO_NETWORK_COMMAND 
#define CONNECT_TO_NETWORK_COMMAND 

#include "command-template.h"

class ConnectToNetworkCommand : public CommandTemplate
{
public:
    bool Execute(Skywire *skywire) override;
    bool ValidateResponse(Skywire *skywire) override;

private:
    static bool isConnectedToNetworkResponseOk(String responseContent);
    static bool isConnectedToNetwork(Skywire* skywire);

};

#endif // CONNECT_TO_NETWORK_COMMAND