#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "esp_mac.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_now.h"

#include "rom/ets_sys.h"
#include "esp_netif.h"

#include "csi_manager.h"

static const char *csi_send_tag = "send_manager";

uint8_t CSI_SEND_MAC[6] = {0x1A, 0xFF, 0x35, 0x24, 0x46, 0x68};

esp_now_peer_info_t peer = {
    .channel   = CONFIG_LESS_INTERFERENCE_CHANNEL,
    .ifidx     = WIFI_IF_STA,
    .encrypt   = false,
    .peer_addr = {0x1A, 0xFE, 0x34, 0x23, 0x45, 0x67},
};

void csi_send_task(void *arg)
{
    (void)arg;

    // TODO: add PMK to the menuconfig
    //ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t *)"pmk1234567890123"));

    ESP_ERROR_CHECK(esp_now_add_peer(&peer));

    ESP_LOGI(csi_send_tag, "================ CSI SEND ================");
    ESP_LOGI(csi_send_tag, "wifi_channel: %d, send_frequency: %d, mac: " MACSTR,
             CONFIG_LESS_INTERFERENCE_CHANNEL, CONFIG_SEND_FREQUENCY, MAC2STR(CSI_SEND_MAC));

    for (uint8_t data = 0;;data++)
    {
        ESP_LOGD(csi_send_tag, "Sending data to " MACSTR, MAC2STR(peer.peer_addr));
        ESP_ERROR_CHECK(esp_now_send(peer.peer_addr, &data, sizeof(uint8_t)));
        vTaskDelay(CONFIG_SEND_FREQUENCY / portTICK_PERIOD_MS);
    }
}

static const char *csi_recv_tag = "recv_manager";

void csi_recv_cb(void *ctx, wifi_csi_info_t *info)
{
    if (!info || !info->buf)
    {
        ESP_LOGW(csi_recv_tag, "<%s> wifi_csi_cb", esp_err_to_name(ESP_ERR_INVALID_ARG));
        return;
    }

    if (memcmp(info->mac, CSI_SEND_MAC, 6))
    {
        return;
    }

    static int s_count = 0;
    const wifi_pkt_rx_ctrl_t *rx_ctrl = &info->rx_ctrl;

    if (!s_count)
    {
        ESP_LOGI(csi_recv_tag, "================ CSI RECV ================");
        ets_printf("type,id,mac,rssi,rate,sig_mode,mcs,bandwidth,smoothing,not_sounding,aggregation,stbc,fec_coding,sgi,noise_floor,ampdu_cnt,channel,secondary_channel,local_timestamp,ant,sig_len,rx_state,len,first_word,data\n");
    }

    ets_printf("CSI_DATA,%d," MACSTR ",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            s_count++, MAC2STR(info->mac), rx_ctrl->rssi, rx_ctrl->rate, rx_ctrl->sig_mode,
            rx_ctrl->mcs, rx_ctrl->cwb, rx_ctrl->smoothing, rx_ctrl->not_sounding,
            rx_ctrl->aggregation, rx_ctrl->stbc, rx_ctrl->fec_coding, rx_ctrl->sgi,
            rx_ctrl->noise_floor, rx_ctrl->ampdu_cnt, rx_ctrl->channel, rx_ctrl->secondary_channel,
            rx_ctrl->timestamp, rx_ctrl->ant, rx_ctrl->sig_len, rx_ctrl->rx_state);

    ets_printf(",%d,%d,\"[%d", info->len, info->first_word_invalid, info->buf[0]);

    for (int i = 1; i < info->len; i++)
    {
        ets_printf(",%d", info->buf[i]);
    }

    ets_printf("]\"\n");
}