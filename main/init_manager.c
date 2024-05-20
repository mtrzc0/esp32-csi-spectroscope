#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include "nvs_flash.h"
#include "esp_mac.h"
#include "esp_log.h"

#include "init_manager.h"
#include "main.h"
#include "task_prio.h"

#if defined CONFIG_CSI_APP_RECEIVER
#include "recv_manager.h"
#endif

ESP_EVENT_DEFINE_BASE(INIT_MANAGER_EVENTS);

const char *init_tag = "init_manager";

void wifi_init_task(void *arg)
{
    (void)arg;

    ESP_ERROR_CHECK(esp_netif_init());
    wifi_init_config_t wifi_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_bandwidth(ESP_IF_WIFI_STA, WIFI_BW_HT40));
    ESP_ERROR_CHECK(esp_wifi_start());

    // TODO: Add rate if needed
    // ESP_ERROR_CHECK(esp_wifi_config_espnow_rate(ESP_IF_WIFI_STA, WIFI_PHY_RATE_MCS0_SGI));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    ESP_ERROR_CHECK(esp_now_init());

    // TODO: add the channel and MAC address configuration to the menuconfig
    // ESP_ERROR_CHECK(esp_wifi_set_channel(CONFIG_LESS_INTERFERENCE_CHANNEL, WIFI_SECOND_CHAN_BELOW));

#if defined CONFIG_CSI_APP_SENDER
    ESP_ERROR_CHECK(esp_wifi_set_mac(WIFI_IF_STA, CONFIG_CSI_SEND_MAC));
#elif defined CONFIG_CSI_APP_RECEIVER
    ESP_ERROR_CHECK(esp_wifi_set_mac(WIFI_IF_STA, CONFIG_CSI_RECV_MAC));
#endif

    vTaskDelete(NULL);
}

void csi_recv_init_task(void *arg)
{
    (void)arg;
#if defined CONFIG_CSI_APP_RECEIVER
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
#endif
    vTaskDelete(NULL);
}

void nvs_init_task(void *arg)
{
    (void)arg;
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    vTaskDelete(NULL);
}

/**
 * @brief Event handler for the initialization events.
 *
 * This function is an event handler for the initialization events. It is called when an event is posted to the INIT_MANAGER_EVENTS event base.
 * It handles four types of events: WIFI_INIT_SUCCESS_EVENT, WIFI_INIT_FAIL_EVENT, ESP_NOW_INIT_SUCCESS_EVENT, and ESP_NOW_INIT_FAIL_EVENT.
 * For any other event, it logs a warning message.
 *
 * @param arg A pointer to the argument data for the event. This is not used in this function.
 * @param event_base The base of the event. This function only handles events with the base INIT_MANAGER_EVENTS.
 * @param event_id The ID of the event. This function handles events with the IDs WIFI_INIT_SUCCESS_EVENT, WIFI_INIT_FAIL_EVENT, ESP_NOW_INIT_SUCCESS_EVENT, and ESP_NOW_INIT_FAIL_EVENT.
 * @param event_data A pointer to the event data. This is not used in this function.
 */
static void inits_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    (void)arg;
    (void)event_data;
    if (event_base == INIT_MANAGER_EVENTS)
    {
        switch (event_id)
        {
        case WIFI_INIT_SUCCESS_EVENT:
            ESP_LOGD(init_tag, "WIFI init success.");
            break;
        case WIFI_INIT_FAIL_EVENT:
            ESP_LOGE(init_tag, "WIFI init failed.");
            break;
        case CSI_RECV_INIT_SUCCESS_EVENT:
            ESP_LOGI(init_tag, "CSI recv init success.");
            break;
        case CSI_RECV_INIT_FAIL_EVENT:
            ESP_LOGE(init_tag, "CSI recv init failed.");
            break;
        default:
            ESP_LOGW(init_tag, "Unknown event");
            break;
        }
    }
}

void run_init_task(void *arg)
{
    (void)arg;
    ESP_LOGI(init_tag, "Initializing modules.");

    ESP_ERROR_CHECK(esp_event_handler_instance_register(INIT_MANAGER_EVENTS, ESP_EVENT_ANY_ID, inits_handler, NULL, NULL));

    // create tasks for each module
    BaseType_t wifi_ret = xTaskCreatePinnedToCore(wifi_init_task, init_tag, configMINIMAL_STACK_SIZE + 2048, NULL, WIFI_INIT_TP, NULL, 0);
    if (wifi_ret != pdPASS)
        esp_event_post(INIT_MANAGER_EVENTS, WIFI_INIT_FAIL_EVENT, NULL, 0, portMAX_DELAY);
    else
        esp_event_post(INIT_MANAGER_EVENTS, WIFI_INIT_SUCCESS_EVENT, NULL, 0, portMAX_DELAY);

    BaseType_t nvs_ret = xTaskCreatePinnedToCore(nvs_init_task, init_tag, configMINIMAL_STACK_SIZE + 2048, NULL, NVS_INIT_TP, NULL, 0);
    if (nvs_ret != pdPASS)
        esp_event_post(INIT_MANAGER_EVENTS, NVS_INIT_FAIL_EVENT, NULL, 0, portMAX_DELAY);
    else
        esp_event_post(INIT_MANAGER_EVENTS, NVS_INIT_SUCCESS_EVENT, NULL, 0, portMAX_DELAY);

 #if defined CONFIG_CSI_APP_RECEIVER
    BaseType_t csi_ret = xTaskCreate(csi_recv_init_task, init_tag, configMINIMAL_STACK_SIZE + 2048, NULL, CSI_INIT_TP, NULL);
    if (csi_ret != pdPASS)
        esp_event_post(INIT_MANAGER_EVENTS, CSI_RECV_INIT_FAIL_EVENT, NULL, 0, portMAX_DELAY);
    else
        esp_event_post(INIT_MANAGER_EVENTS, CSI_RECV_INIT_SUCCESS_EVENT_INIT_SUCCESS_EVENT, NULL, 0, portMAX_DELAY);
#endif

    vTaskDelete(NULL);
}
