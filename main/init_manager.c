#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include "nvs_flash.h"
#include "esp_mac.h"
#include "esp_log.h"

#include "init_manager.h"
#include "csi_manager.h"
#include "main.h"

const char *init_tag = "init_manager";

void wifi_init()
{
    ESP_LOGD(init_tag, "WIFI init start.");
    ESP_ERROR_CHECK(esp_netif_init());
    const wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_bandwidth(ESP_IF_WIFI_STA, WIFI_BW_HT40));
    ESP_ERROR_CHECK(esp_wifi_start());

    // TODO: replace this with NOT deprecated function
    ESP_ERROR_CHECK(esp_wifi_config_espnow_rate(ESP_IF_WIFI_STA, WIFI_PHY_RATE_MCS0_SGI));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

    ESP_ERROR_CHECK(esp_wifi_set_channel(CONFIG_LESS_INTERFERENCE_CHANNEL, WIFI_SECOND_CHAN_BELOW));
    ESP_ERROR_CHECK(esp_now_init());

    ESP_ERROR_CHECK(esp_wifi_set_mac(WIFI_IF_STA, CSI_SEND_MAC));

    esp_event_post(APP_MAIN_EVENTS, WIFI_INIT_SUCCESS_EVENT, NULL, 0, portMAX_DELAY);
}

void csi_recv_init()
{
#if defined CONFIG_CSI_APP_TYPE_RECEIVER
    ESP_LOGD(init_tag, "CSI RECV init start");
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
    // ESP_ERROR_CHECK(esp_wifi_set_promiscuous_rx_cb(g_wifi_radar_config->wifi_sniffer_cb));

    /**< default config */
    wifi_csi_config_t cfg = {
        .lltf_en           = true,
        .htltf_en          = true,
        .stbc_htltf2_en    = true,
        .ltf_merge_en      = true,
        .channel_filter_en = true,
        .manu_scale        = false,
        .shift             = false,
    };
    ESP_ERROR_CHECK(esp_wifi_set_csi_config(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_csi_rx_cb(csi_recv_cb, NULL));
    ESP_ERROR_CHECK(esp_wifi_set_csi(true));

    esp_event_post(APP_MAIN_EVENTS, CSI_RECV_INIT_SUCCESS_EVENT, NULL, 0, portMAX_DELAY);
#endif
}

void nvs_init()
{
    ESP_LOGD(init_tag, "NVS init start.");
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    if (ret != ESP_OK)
    {
        ESP_LOGE(init_tag, "NVS init failed.");
        return;
    }

    esp_event_post(APP_MAIN_EVENTS, NVS_INIT_SUCCESS_EVENT, NULL, 0, portMAX_DELAY);
}


