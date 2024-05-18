#include <esp_log.h>
#include <esp_event.h>

#include "main.h"
#include "init_manager.h"
#include "task_prio.h"

#if defined CONFIG_CSI_APP_TYPE_SENDER
#include "send_manager.h"
#endif

ESP_EVENT_DEFINE_BASE(APP_MAIN_EVENTS);

const char* main_tag = "main";

/**
 * @brief Event handler for the initialization manager.
 *
 * This function is an event handler for the initialization manager. It is called when an event is posted to the APP_MAIN_EVENTS event base.
 * It handles two types of events: INIT_MANAGER_SUCCESS_EVENT and INIT_MANAGER_FAIL_EVENT. For any other event, it logs a warning message.
 *
 * @param arg A pointer to the argument data for the event. This is not used in this function.
 * @param event_base The base of the event. This function only handles events with the base APP_MAIN_EVENTS.
 * @param event_id The ID of the event. This function handles events with the IDs INIT_MANAGER_SUCCESS_EVENT and INIT_MANAGER_FAIL_EVENT.
 * @param event_data A pointer to the event data. This is not used in this function.
 */
static void init_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
	(void) arg;
	(void) event_data;
	if (event_base == APP_MAIN_EVENTS) {
		switch (event_id) {
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
	ESP_ERROR_CHECK(esp_event_handler_instance_register(APP_MAIN_EVENTS, ESP_EVENT_ANY_ID, init_handler, NULL, NULL));

	BaseType_t init_manager_ret = xTaskCreate(run_init_task, main_tag, configMINIMAL_STACK_SIZE + 2048, NULL, INIT_MANAGER_TP, NULL);
	if (init_manager_ret != pdPASS)
		esp_event_post(APP_MAIN_EVENTS, INIT_MANAGER_FAIL_EVENT, NULL, 0, portMAX_DELAY);
	else
		esp_event_post(APP_MAIN_EVENTS, INIT_MANAGER_SUCCESS_EVENT, NULL, 0, portMAX_DELAY);
}