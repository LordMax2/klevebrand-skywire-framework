#include "skywire-command-at.h"

AtSkywireCommand::AtSkywireCommand(HardwareSerial *skywire, const bool debug_mode, const OnCompletedFunction on_completed_function) : SkywireCommand(skywire, "AT", debug_mode, on_completed_function)
{
}