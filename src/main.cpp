#include "Arduino.h"
#include "skywire-command-startup-worker.h"
#include "skywire-command-tcp-gps-step-worker.h"

void setup()
{
    Serial.begin(115200);

    Serial.println(F("Starting Skywire modem..."));

    constexpr size_t receive_buffer_size = 256;
    constexpr size_t socket_send_message_size = 160;
    constexpr size_t socket_receive_size = 128;

    SkywireCommandStartupWorker<receive_buffer_size> startup_worker(&Serial3, false);
    SkywireTcpGpsStepWorker<receive_buffer_size, socket_send_message_size, socket_receive_size> tcp_gps_worker(
        &Serial3,
        "flightcontroltower.klevebrand.se",
        13000,
        20000,
        false);

    Serial3.begin(115200);

    while (!startup_worker.run())
    {
    }

    Serial.println(F("Startup completed!"));

    while (true)
    {
        const char payload_to_send[] = "1;1337;true;1;2;3;99;120.5;59.8586;17.6389;42.5;1013.2;2;7";
        tcp_gps_worker.setPayloadToSend(payload_to_send);
        tcp_gps_worker.run();
    }
}

void loop()
{
}
