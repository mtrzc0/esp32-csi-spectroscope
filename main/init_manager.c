#include <esp_event.h>
#include <esp_wifi.h>
#include "nvs_flash.h"
#include "esp_mac.h"
#include "esp_log.h"

#include "init_manager.h"
#include "main.h"
#include "task_prio.h"

ESP_EVENT_DEFINE_BASE(INIT_MANAGER_EVENTS);

const char *init_tag = "init_manager";

void wifi_init()
{
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(esp_netif_init());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_bandwidth(ESP_IF_WIFI_STA, WIFI_BW_HT40));
    ESP_ERROR_CHECK(esp_wifi_start());

    // TODO: add this to esp_now_init
    // ESP_ERROR_CHECK(esp_wifi_config_espnow_rate(ESP_IF_WIFI_STA, WIFI_PHY_RATE_MCS0_SGI));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

    // TODO: add the channel and MAC address configuration here
    // ESP_ERROR_CHECK(esp_wifi_set_channel(CONFIG_LESS_INTERFERENCE_CHANNEL, WIFI_SECOND_CHAN_BELOW));
    // ESP_ERROR_CHECK(esp_wifi_set_mac(WIFI_IF_STA, CONFIG_CSI_SEND_MAC));
}

static void init_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    (void)arg;
    (void)event_data;
    if (event_base == INIT_MANAGER_EVENTS) {
        switch (event_id) {
        case WIFI_INIT_SUCCESS_EVENT:
            ESP_LOGD(init_tag, "WIFI init success.");
            break;
        case WIFI_INIT_FAIL_EVENT:
            ESP_LOGE(init_tag, "WIFI init failed.");
            break;
        default:
            ESP_LOGW(init_tag, "Unknown event");
            break;
        }
    }
}

void run_init_manager(void *arg)
{
    (void)arg;
    ESP_LOGD(init_tag, "Initializing modules.");
    // TODO: Initialize modules here.
    // - WIFI
    // - CSI
    // - NVS
    // - ESP NOW

    ESP_ERROR_CHECK(esp_event_handler_instance_register(INIT_MANAGER_EVENTS, ESP_EVENT_ANY_ID, init_handler, NULL, NULL));
    BaseType_t wifi_ret = xTaskCreate(wifi_init, init_tag, configMINIMAL_STACK_SIZE + 2048, NULL, RUN_WIFI_INIT_TP, NULL);
    if (wifi_ret != pdPASS)
        esp_event_post(INIT_MANAGER_EVENTS, WIFI_INIT_FAIL_EVENT, NULL, 0, portMAX_DELAY);
    else
        esp_event_post(INIT_MANAGER_EVENTS, WIFI_INIT_SUCCESS_EVENT, NULL, 0, portMAX_DELAY);

    
    // esp_err_t ret = nvs_flash_init();
    // if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     ESP_ERROR_CHECK(nvs_flash_erase());
    //     ret = nvs_flash_init();
    // }
    // ESP_ERROR_CHECK(ret);
    //
    // wifi_init();
    // wifi_csi_init();

    esp_event_post(APP_MAIN_EVENTS, INIT_MANAGER_SUCCESS_EVENT, NULL, 0, portMAX_DELAY);
    vTaskDelete(NULL);
}
