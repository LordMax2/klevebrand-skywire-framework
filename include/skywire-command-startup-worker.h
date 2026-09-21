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
    AtSkywireCommand _at_command;
    SkywireAtEngine _cmee_command;
    DisableEchoSkywireCommand _disable_echo_command;
    SkywireAtEngine _flow_control_command;
    SkywireAtEngine _interface_control_command;
    SetApnHologramSkywireCommand _set_apn_command;
    NetworkConnectSkywireCommand _network_connect_command;
    EnablePacketDataSkywireCommand _enable_packet_data_command;
    EnableGpsSkywireCommand _enable_gps_command;
};

static_assert(SkywireWorkerConcept<SkywireCommandStartupWorker>, "SkywireCommandStartupWorker doesnt implement the concept");

#endif
