#include "skywire-command-at.h"

AtSkywireCommand::AtSkywireCommand(HardwareSerial *skywire, bool debug_mode, OnCompletedFunction on_completed_function) : SkywireCommand(skywire, "AT", debug_mode, on_completed_function)
{
}