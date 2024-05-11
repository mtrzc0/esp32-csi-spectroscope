#include <esp_log.h>
#include <esp_event.h>

#include "main.h"
#include "init_manager.h"
#include "task_prio.h"

ESP_EVENT_DEFINE_BASE(APP_MAIN_EVENTS);

const char* main_tag = "app_tag";

static void init_manager_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
	(void) arg;
	(void) event_data;
	if (event_base == APP_MAIN_EVENTS) {
		switch (event_id) {
		case INIT_MANAGER_SUCCESS_EVENT:
			ESP_LOGD(main_tag, "Init manager success.");
			break;
		case INIT_MANAGER_FAIL_EVENT:
			ESP_LOGE(main_tag, "Init manager failed.");
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
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	ESP_ERROR_CHECK(esp_event_handler_instance_register(APP_MAIN_EVENTS, INIT_MANAGER_RUN_EVENT, init_manager_handler, NULL, NULL));
	BaseType_t init_manager_ret = xTaskCreate(run_init_manager, main_tag, configMINIMAL_STACK_SIZE + 2048, NULL, RUN_INIT_MANAGER_TP, NULL);
	if (init_manager_ret != pdPASS)
		esp_event_post(APP_MAIN_EVENTS, INIT_MANAGER_FAIL_EVENT, NULL, 0, portMAX_DELAY);
	else
		esp_event_post(APP_MAIN_EVENTS, INIT_MANAGER_SUCCESS_EVENT, NULL, 0, portMAX_DELAY);
}