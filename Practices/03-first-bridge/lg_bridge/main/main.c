#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "operator1_module0.h"   

static const char *TAG = "SCADE";
static outC_operator1_module0 ctx;
static swan_int32 cont;
static swan_bool green, yellow, red;

#define LED_RED 25
#define LED_YEL 26
#define LED_GRN 27

void app_main(void)
{
    gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_YEL, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_GRN, GPIO_MODE_OUTPUT);

    operator1_reset_module0(&ctx);
    while (1) {
        operator1_module0(&cont, &green, &yellow, &red, &ctx);
        gpio_set_level(LED_RED,    red);
        gpio_set_level(LED_YEL,    yellow);
        gpio_set_level(LED_GRN,    green);
        ESP_LOGI(TAG, "tick=%d  G=%d Y=%d R=%d",
                 (int)cont, (int)green, (int)yellow, (int)red);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}