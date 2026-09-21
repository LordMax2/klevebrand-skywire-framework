#include "skywire-command-startup-worker.h"

SkywireCommandStartupWorker::SkywireCommandStartupWorker(HardwareSerial *skywire_serial, const bool debug_mode)
    : _stepper(5000),
      _at_command(skywire_serial, debug_mode, nullptr),
      _cmee_command(skywire_serial, F("AT+CMEE=2"), debug_mode, nullptr),
      _disable_echo_command(skywire_serial, debug_mode, nullptr),
      _flow_control_command(skywire_serial, F("AT&K0"), debug_mode, nullptr),
      _interface_control_command(skywire_serial, F("AT+IFC=0,0"), debug_mode, nullptr),
      _set_apn_command(skywire_serial, debug_mode, nullptr),
      _network_connect_command(skywire_serial, debug_mode, nullptr),
      _enable_packet_data_command(skywire_serial, debug_mode, nullptr),
      _enable_gps_command(skywire_serial, debug_mode, nullptr)
{
}

void SkywireCommandStartupWorker::reset()
{
    _stepper.reset(
        _at_command,
        _cmee_command,
        _disable_echo_command,
        _flow_control_command,
        _interface_control_command,
        _set_apn_command,
        _network_connect_command,
        _enable_packet_data_command,
        _enable_gps_command);
}

bool SkywireCommandStartupWorker::run()
{
    if (_stepper.isFinished())
    {
        return true;
    }

    const SkywireStepResult result = _stepper.stepCurrent(
        _at_command,
        _cmee_command,
        _disable_echo_command,
        _flow_control_command,
        _interface_control_command,
        _set_apn_command,
        _network_connect_command,
        _enable_packet_data_command,
        _enable_gps_command);

    if (result == SkywireStepResult::TimedOut)
    {
        reset();
        SkywireAtEngine::rebeginModem();
    }

    return false;
}
