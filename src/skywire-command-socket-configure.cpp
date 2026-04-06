#include "skywire-command-socket-configure.h"

SocketConfigureSkywireCommand::SocketConfigureSkywireCommand(HardwareSerial *skywire,
                                                             const bool debug_mode,
                                                             const OnCompletedFunction on_completed_function)
    : SkywireCommand(skywire, "AT#SCFG=1,1,300,90,600,50", debug_mode, on_completed_function) {
}
