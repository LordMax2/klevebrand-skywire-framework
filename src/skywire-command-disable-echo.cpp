#include "skywire-command-disable-echo.h"

DisableEchoSkywireCommand::DisableEchoSkywireCommand(HardwareSerial* skywire, bool debug_mode, void (*on_completed_function)(String &result_content))
    : SkywireCommand(skywire, "ATE0", debug_mode, on_completed_function)
{
}
