#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "FlightPhase_module0.h" 
#define BTN 0 
#define LED_GRN 25
#define LED_YEL 26
#define LED_RED 27
static const char *TAG = "RTOS";
static QueueHandle_t q_evt; 
static QueueHandle_t q_led; 

static void input_task(void *arg)
{
gpio_config_t io = { .pin_bit_mask = 1ULL << BTN,
.mode = GPIO_MODE_INPUT, .pull_up_en = GPIO_PULLUP_ENABLE };
gpio_config(&io);
int prev = 1;
while (1) {
int now = gpio_get_level(BTN);
if (prev == 1 && now == 0) { 
uint8_t ev = 1;
xQueueSend(q_evt, &ev, 0);
}
prev = now;
vTaskDelay(pdMS_TO_TICKS(20)); 
}
}

static void model_task(void *arg)
{
outC_FlightPhase_module0 ctx;
swan_int32 phase;
FlightPhase_reset_module0(&ctx);
TickType_t last = xTaskGetTickCount();
while (1) {
uint8_t ev;
swan_bool next = false;
if (xQueueReceive(q_evt, &ev, 0) == pdTRUE) next = true; 
FlightPhase_module0(next, &phase, &ctx);
int p = (int)phase;
xQueueSend(q_led, &p, 0);
vTaskDelayUntil(&last, pdMS_TO_TICKS(100));
}
}

static void led_task(void *arg)
{
gpio_set_direction(LED_GRN, GPIO_MODE_OUTPUT);
gpio_set_direction(LED_YEL, GPIO_MODE_OUTPUT);
gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
int p, last_p = -1;
while (1) if (xQueueReceive(q_led, &p, portMAX_DELAY) == pdTRUE) {
gpio_set_level(LED_GRN, (p == 0 || p == 1));
gpio_set_level(LED_YEL, (p == 1 || p == 2 || p == 3));
gpio_set_level(LED_RED, (p == 3));
if (p != last_p) { 
const char *n[] = {"GROUND","TAKEOFF","CRUISE","LANDING"};
ESP_LOGI(TAG, "phase=%d %s", p, n[p]);
last_p = p;
}
}
}
void app_main(void)
{
q_evt = xQueueCreate(4, sizeof(uint8_t));
q_led = xQueueCreate(4, sizeof(int));
xTaskCreate(input_task, "input", 2048, NULL, 2, NULL);
xTaskCreate(model_task, "model", 3072, NULL, 3, NULL);
xTaskCreate(led_task, "led", 3072, NULL, 1, NULL);
}