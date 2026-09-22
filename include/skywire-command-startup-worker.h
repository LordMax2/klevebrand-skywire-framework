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

#include "skywire-command-startup-worker.ipp"

#endif
