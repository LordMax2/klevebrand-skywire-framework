#pragma once

#ifndef SKYWIRE_COMMAND_STARTUP_WORKER_H
#define SKYWIRE_COMMAND_STARTUP_WORKER_H

#include "Arduino.h"
#include "skywire_stepper.h"
#include "skywire_at_engine.h"
#include "concept_skywire_worker.h"
#include "skywire-command-at.h"
#include "skywire-command-disable-echo.h"
#include "skywire-command-set-hologram-apn.h"
#include "skywire-command-network-connect.h"
#include "skywire-command-enable-packet-data.h"
#include "skywire-command-enable-gps.h"

#define STARTUP_STEP_COUNT 9

template<size_t RxBufferSize>
class SkywireCommandStartupWorker
{
public:
    explicit SkywireCommandStartupWorker(
        HardwareSerial *skywire_serial,
        bool debug_mode = false);

    bool run();
    void reset();

private:
    SkywireStepper<STARTUP_STEP_COUNT> _stepper;
    AtSkywireCommand<RxBufferSize> _at_command;
    SkywireAtEngine<RxBufferSize> _cmee_command;
    DisableEchoSkywireCommand<RxBufferSize> _disable_echo_command;
    SkywireAtEngine<RxBufferSize> _flow_control_command;
    SkywireAtEngine<RxBufferSize> _interface_control_command;
    SetApnHologramSkywireCommand<RxBufferSize> _set_apn_command;
    NetworkConnectSkywireCommand<RxBufferSize> _network_connect_command;
    EnablePacketDataSkywireCommand<RxBufferSize> _enable_packet_data_command;
    EnableGpsSkywireCommand<RxBufferSize> _enable_gps_command;
};


template<size_t RxBufferSize>
SkywireCommandStartupWorker<RxBufferSize>::SkywireCommandStartupWorker(
    HardwareSerial *skywire_serial,
    const bool debug_mode)
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
    static_assert(SkywireWorkerConcept<SkywireCommandStartupWorker<RxBufferSize>>, "SkywireCommandStartupWorker doesnt implement the concept");
}

template<size_t RxBufferSize>
void SkywireCommandStartupWorker<RxBufferSize>::reset()
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

template<size_t RxBufferSize>
bool SkywireCommandStartupWorker<RxBufferSize>::run()
{
    if (_stepper.hasCompletedAllCommands())
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

    if (result == SkywireStepResult::StepTimedOut)
    {
        reset();
        SkywireAtEngine<RxBufferSize>::rebeginModem();
    }

    return false;
}

#endif
