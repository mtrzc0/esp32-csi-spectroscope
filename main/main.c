#include <esp_log.h>
#include <esp_event.h>

#include "main.h"
#include "init_manager.h"
#include "task_prio.h"
#include "csi_manager.h"

ESP_EVENT_DEFINE_BASE(APP_MAIN_EVENTS);

const char* main_tag = "main";

static uint8_t inits_counter = 0;

static void init_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
	(void) arg;
	(void) event_data;

	if (event_base == APP_MAIN_EVENTS) {
		switch (event_id)
		{
		case INIT_MANAGER_SUCCESS_EVENT:
			ESP_LOGI(main_tag, "Init manager success.");
#if defined CONFIG_CSI_APP_TYPE_SENDER
			BaseType_t sender_ret = xTaskCreate(csi_send_task, main_tag, configMINIMAL_STACK_SIZE + 2048, NULL, CSI_SEND_TP, NULL);
			if (sender_ret != pdPASS)
				ESP_LOGE(main_tag, "CSI send task creation failed.");
#endif
			break;
		case INIT_MANAGER_FAIL_EVENT:
			ESP_LOGE(main_tag, "Init manager failed.");
			break;
        case WIFI_INIT_SUCCESS_EVENT:
        	inits_counter++;
			ESP_LOGI(main_tag, "WIFI init success.");
        	break;
#if defined CONFIG_CSI_APP_TYPE_RECEIVER
        case CSI_RECV_INIT_SUCCESS_EVENT:
        	inits_counter++;
			ESP_LOGI(main_tag, "CSI RECV init success.");
        	break;
#endif
        case NVS_INIT_SUCCESS_EVENT:
        	inits_counter++;
			ESP_LOGI(main_tag, "NVS init success.");
        	break;
        default:
			ESP_LOGW(main_tag, "Unknown event");
        	break;
        }
    }
}

void app_main()
{
	ESP_LOGD(main_tag, "Creating system event loop.");
	esp_event_loop_create_default();
	esp_event_handler_instance_register(APP_MAIN_EVENTS, NVS_INIT_SUCCESS_EVENT, init_handler, NULL, NULL);
	esp_event_handler_instance_register(APP_MAIN_EVENTS, WIFI_INIT_SUCCESS_EVENT, init_handler, NULL, NULL);
#if defined CONFIG_CSI_APP_TYPE_RECEIVER
	esp_event_handler_instance_register(APP_MAIN_EVENTS, CSI_RECV_INIT_SUCCESS_EVENT, init_handler, NULL, NULL);
#endif
	esp_event_handler_instance_register(APP_MAIN_EVENTS, INIT_MANAGER_SUCCESS_EVENT, init_handler, NULL, NULL);
	esp_event_handler_instance_register(APP_MAIN_EVENTS, INIT_MANAGER_FAIL_EVENT, init_handler, NULL, NULL);
	esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, init_handler, NULL, NULL);

	// Initialize all drivers
	nvs_init();
	wifi_init();
	csi_recv_init();

	if (inits_counter == INITS_SUCCESS)
		esp_event_post(APP_MAIN_EVENTS, INIT_MANAGER_SUCCESS_EVENT, NULL, 0, portMAX_DELAY);
	else
		esp_event_post(APP_MAIN_EVENTS, INIT_MANAGER_FAIL_EVENT, NULL, 0, portMAX_DELAY);
}