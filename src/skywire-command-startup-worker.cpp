#include "skywire-command-startup-worker.h"

SkywireCommandStartupWorker::SkywireCommandStartupWorker(HardwareSerial *skywire_serial, const bool debug_mode)
    : _stepper(5000, STARTUP_STEP_COUNT),
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
    _at_command.reset();
    _cmee_command.reset();
    _disable_echo_command.reset();
    _flow_control_command.reset();
    _interface_control_command.reset();
    _set_apn_command.reset();
    _network_connect_command.reset();
    _enable_packet_data_command.reset();
    _enable_gps_command.reset();
    _stepper.resetCursor();
}

SkywireStepperTickResult SkywireCommandStartupWorker::tickCurrentStep()
{
    switch (_stepper.stepCursorIndex())
    {
    case 0:
        return _stepper.tick(_at_command);
    case 1:
        return _stepper.tick(_cmee_command);
    case 2:
        return _stepper.tick(_disable_echo_command);
    case 3:
        return _stepper.tick(_flow_control_command);
    case 4:
        return _stepper.tick(_interface_control_command);
    case 5:
        return _stepper.tick(_set_apn_command);
    case 6:
        return _stepper.tick(_network_connect_command);
    case 7:
        return _stepper.tick(_enable_packet_data_command);
    case 8:
        return _stepper.tick(_enable_gps_command);
    default:
        return SkywireStepperTickResult::Finished;
    }
}

bool SkywireCommandStartupWorker::run()
{
    if (_stepper.isFinished())
    {
        return true;
    }

    const SkywireStepperTickResult result = tickCurrentStep();

    if (result == SkywireStepperTickResult::TimedOut)
    {
        reset();
        SkywireAtEngine::rebeginModem();
    }

    return false;
}
