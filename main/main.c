#include <esp_log.h>

const char* main_tag = "app";

void app_main()
{
	ESP_LOGD(main_tag, "test");
}