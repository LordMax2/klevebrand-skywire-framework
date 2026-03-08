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

#define STEP_COUNT 7

class SkywireCommandStartupWorker : public SkywireCommandWorker
{
public:
    SkywireCommandStartupWorker(
        HardwareSerial* skywire_serial,
        bool debug_mode = false);

    static void onAtCommandCompleted(String &result_content);

    static void onDisableEchoCommandCompleted(String &result_content);

    static void onSetApnCommandCompleted(String &result_content);

    static void onNetworkConnectCommandCompleted(String &result_content);

    static void onEnablePacketDataCommandCompleted(String &result_content);

    static void onEnableGpsCommandCompleted(String &result_content);

    bool run() override;
};

#endif 