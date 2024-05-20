#ifndef CSI_MANAGER_H
#define CSI_MANAGER_H

#include <esp_now.h>
#include "esp_wifi.h"

// TODO: document this function
void csi_send_task(void *arg);

extern esp_now_peer_info_t peer;
extern uint8_t CSI_SEND_MAC[6];

// TODO: document this function
void csi_recv_task(void *ctx, wifi_csi_info_t *data);

#endif //CSI_MANAGER_H
