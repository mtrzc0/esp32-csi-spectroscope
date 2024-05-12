#ifndef ESP_CSI_SENSOR_FUSION_RECV_MANAGER_H
#define ESP_CSI_SENSOR_FUSION_RECV_MANAGER_H

#include "esp_wifi.h"

// TODO: document this function
void csi_recv_task(void *ctx, wifi_csi_info_t *data);

#endif //ESP_CSI_SENSOR_FUSION_RECV_MANAGER_H
