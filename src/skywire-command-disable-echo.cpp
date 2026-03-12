#include "skywire-command-disable-echo.h"

DisableEchoSkywireCommand::DisableEchoSkywireCommand(HardwareSerial *skywire, const bool debug_mode, const OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "ATE0", debug_mode, on_completed_function)
{
}
