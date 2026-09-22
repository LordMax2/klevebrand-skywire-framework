#pragma once

#ifndef SKYWIRE_CONFIG_H
#define SKYWIRE_CONFIG_H

#ifndef SKYWIRE_RX_BUFFER_SIZE
#define SKYWIRE_RX_BUFFER_SIZE 256
#endif

#ifndef SOCKET_SEND_MESSAGE_SIZE
// "SetDroneState|1337|" plus the 128-byte telemetry payload Maxfly builds.
#define SOCKET_SEND_MESSAGE_SIZE 160
#endif

#ifndef SKYWIRE_SRECV_SIZE
#define SKYWIRE_SRECV_SIZE 128
#endif

#ifndef SKYWIRE_ENABLE_HTTP
#define SKYWIRE_ENABLE_HTTP 0
#endif

#ifndef HTTP_SND_PATH_SIZE
#define HTTP_SND_PATH_SIZE 64
#endif

#ifndef HTTP_SND_PAYLOAD_TO_SEND_SIZE
#define HTTP_SND_PAYLOAD_TO_SEND_SIZE 128
#endif

#endif
