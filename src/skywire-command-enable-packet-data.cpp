#include "skywire-command-enable-packet-data.h"
#include "skywire_strstr_p.h"

EnablePacketDataSkywireCommand::EnablePacketDataSkywireCommand(
    HardwareSerial *skywire,
    const bool debug_mode,
    const OnCompletedFunction on_completed_function)
    : _at(skywire, F("AT#SGACT=1,1"), debug_mode, on_completed_function)
{
}

bool EnablePacketDataSkywireCommand::okReceived() const
{
    char *const rx_buffer = SkywireAtEngine::getRxBuffer();

    return _at.okReceived() ||
           skywireContainsP(rx_buffer, PSTR("ERROR")) ||
           skywireContainsP(rx_buffer, PSTR("+CME ERROR: context already activated"));
}

SkywireResponseResult_t EnablePacketDataSkywireCommand::process()
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

bool EnablePacketDataSkywireCommand::completed() const
{
    return _at.hasMarkedCompleted() || (_at.hasSent() && okReceived());
}
