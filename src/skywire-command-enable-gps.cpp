#include "skywire-command-enable-gps.h"
#include "skywire_flash_string.h"

EnableGpsSkywireCommand::EnableGpsSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT$GPSP=1"), debug_mode, on_completed_function)
{
}

bool EnableGpsSkywireCommand::okReceived() const
{
    return _at.okReceived() || skywireContainsFlashString(SkywireAtEngine::getRxBuffer(), PSTR("ERROR"));
}

SkywireResponseResult_t EnableGpsSkywireCommand::process()
{
    char *const rx_buffer = SkywireAtEngine::getRxBuffer();

    if (completed())
    {
        return {true, rx_buffer};
    }

    if (!_at.waitForSendThenRead())
    {
        return {false, rx_buffer};
    }

    if (!okReceived())
    {
        return {false, rx_buffer};
    }

    _at.notifyCompletedIfNeeded();
    _at.setCompleted(true);

    return {true, rx_buffer};
}

bool EnableGpsSkywireCommand::completed() const
{
    return _at.hasMarkedCompleted() || (_at.hasSent() && okReceived());
}
