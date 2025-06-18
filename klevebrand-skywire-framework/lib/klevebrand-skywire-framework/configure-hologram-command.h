#ifndef CONFIGURE_HOLOGRAM_COMMAND
#define CONFIGURE_HOLOGRAM_COMMAND

#include "command-template.h"

class ConfigureHologramCommand : public CommandTemplate
{
public:
    bool Execute(Skywire* skywire) override;
    bool ValidateResponse(Skywire* skywire) override;
private:
    static bool isHologramApnSuccessfullyConfigured(Skywire* skywire);
    static bool isHologramApnSuccessfullyConfiguredResponseOk(String responseContent);
};

#endif // CONFIGURE_HOLOGRAM_COMMAND