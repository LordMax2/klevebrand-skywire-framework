#ifndef SKYWIRE_STEP_STARTUP_WORKER_H
#define SKYWIRE_STEP_STARTUP_WORKER_H

#include "Arduino.h"
#include "skywire-command-worker.h"
#include "skywire-command-at.h"
#include "skywire-command-disable-echo.h"
#include "skywire-command-set-hologram-apn.h"
#include "skywire-command-network-connect.h"
#include "skywire-command-enable-packet-data.h"
#include "skywire-command-enable-gps.h"

#define STARTUP_STEP_COUNT 9

class SkywireCommandStartupWorker : public SkywireCommandWorker
{
public:
    explicit SkywireCommandStartupWorker(
        HardwareSerial *skywire_serial,
        bool debug_mode = false);

    bool run() override;

private:
    AtSkywireCommand at_command;
    SkywireCommand cmee_command;
    DisableEchoSkywireCommand disable_echo_command;
    SkywireCommand flow_control_command;
    SkywireCommand interface_control_command;
    SetApnHologramSkywireCommand set_apn_command;
    NetworkConnectSkywireCommand network_connect_command;
    EnablePacketDataSkywireCommand enable_packet_data_command;
    EnableGpsSkywireCommand enable_gps_command;
};

#endif
