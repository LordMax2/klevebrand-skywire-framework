#include "skywire-command-disable-echo.h"

DisableEchoSkywireCommand::DisableEchoSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "ATE0", debug_mode, on_completed_function)
{
}
