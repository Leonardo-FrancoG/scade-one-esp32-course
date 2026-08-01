#include "freertos/FreeRTOS.h"
 #include "freertos/task.h"
 #include "driver/gpio.h"
 #include "esp_log.h"
 #define LED_PIN GPIO_NUM_2
 static const char *TAG = "LG_ENV";
 void app_main(void)
 {
 ESP_LOGI(TAG, "lg_env_check | ESP-IDF %s", IDF_VER);
 gpio_reset_pin(LED_PIN);
 gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
 uint32_t cycle = 0;
 while (1) {
 gpio_set_level(LED_PIN, 1);
 ESP_LOGI(TAG, "LED -> ON | cycle=%lu", (unsigned long)cycle);
 vTaskDelay(pdMS_TO_TICKS(500));
 gpio_set_level(LED_PIN, 0);
 ESP_LOGI(TAG, "LED -> OFF | cycle=%lu", (unsigned long)cycle);
 vTaskDelay(pdMS_TO_TICKS(500));
 cycle++;
 }
 }