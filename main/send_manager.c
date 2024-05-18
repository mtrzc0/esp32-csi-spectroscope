#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "nvs_flash.h"

#include "esp_mac.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_now.h"

static const char *csi_send_tag = "send_manager";

#define CONFIG_LESS_INTERFERENCE_CHANNEL    11
#define CONFIG_SEND_FREQUENCY               100

static uint8_t CSI_SEND_MAC[6] = CONFIG_CSI_SEND_MAC;

void csi_send_task(void *arg)
{
    (void)arg;

    // TODO: add PMK to the menuconfig
    //ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t *)"pmk1234567890123"));

    esp_now_peer_info_t recv_peer = {
        .channel   = CONFIG_LESS_INTERFERENCE_CHANNEL,
        .ifidx     = WIFI_IF_STA,    
        .encrypt   = false,
        .peer_addr = CONFIG_CSI_RECV_MAC, // TEST IF MAC CAN BE string
    };

    ESP_ERROR_CHECK(esp_now_add_peer(&recv_peer));

    ESP_LOGI(csi_send_tag, "================ CSI SEND ================");
    ESP_LOGI(csi_send_tag, "wifi_channel: %d, send_frequency: %d, mac: " MACSTR,
             CONFIG_LESS_INTERFERENCE_CHANNEL, CONFIG_SEND_FREQUENCY, MAC2STR(CSI_SEND_MAC));

    for (uint8_t count = 0; ;++count)
    {
        esp_err_t ret = esp_now_send(recv_peer.peer_addr, &count, sizeof(uint8_t));

        if(ret != ESP_OK)
        {
            ESP_LOGW(csi_send_tag, "<%s> ESP-NOW send error", esp_err_to_name(ret));
        }

        usleep(1000 * 1000 / CONFIG_SEND_FREQUENCY);
    }
}
