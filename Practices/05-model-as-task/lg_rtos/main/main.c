#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "operator1_module0.h" 
#define LED_RED 25
#define LED_YEL 26
#define LED_GRN 27
#define LED_HB 2
#define U2_PORT UART_NUM_2
#define U2_TX 4
#define U2_RX 5
#define HDR 0xAA
static const char *TAG = "RTOS";
typedef struct { int tick; uint8_t state; } lg_msg_t; 
static QueueHandle_t q_led, q_tlm;
static SemaphoreHandle_t logmux;

static void model_task(void *arg)
{
outC_operator1_module0 ctx; swan_int32 cont; swan_bool g,y,r;
operator1_reset_module0(&ctx);
TickType_t last = xTaskGetTickCount();
while (1) {
operator1_module0(&cont, &g, &y, &r, &ctx);
lg_msg_t m = { .tick=(int)cont, .state=(g?1:0)|(y?2:0)|(r?4:0) };
xQueueSend(q_led, &m, 0); 
xQueueSend(q_tlm, &m, 0);
vTaskDelayUntil(&last, pdMS_TO_TICKS(100));
}
}

static void led_task(void *arg)
{
gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
gpio_set_direction(LED_YEL, GPIO_MODE_OUTPUT);
gpio_set_direction(LED_GRN, GPIO_MODE_OUTPUT);
lg_msg_t m;
while (1) if (xQueueReceive(q_led, &m, portMAX_DELAY) == pdTRUE) {
gpio_set_level(LED_GRN, m.state & 1);
gpio_set_level(LED_YEL, (m.state >> 1) & 1);
gpio_set_level(LED_RED, (m.state >> 2) & 1);
xSemaphoreTake(logmux, portMAX_DELAY);
ESP_LOGI(TAG, "LED tick=%d G=%d Y=%d R=%d", m.tick,
m.state&1,(m.state>>1)&1,(m.state>>2)&1);
xSemaphoreGive(logmux);
}
}

static void telemetry_task(void *arg)
{
uart_config_t cfg = { .baud_rate=115200, .data_bits=UART_DATA_8_BITS,
.parity=UART_PARITY_DISABLE, .stop_bits=UART_STOP_BITS_1,
.flow_ctrl=UART_HW_FLOWCTRL_DISABLE, .source_clk=UART_SCLK_DEFAULT };
uart_driver_install(U2_PORT, 256, 0, 0, NULL, 0);
uart_param_config(U2_PORT, &cfg);
uart_set_pin(U2_PORT, U2_TX, U2_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
lg_msg_t m;
while (1) if (xQueueReceive(q_tlm, &m, portMAX_DELAY) == pdTRUE) {
uint8_t tx[4] = { HDR, (uint8_t)m.tick, m.state, 0 };
tx[3] = tx[0]^tx[1]^tx[2];
uart_write_bytes(U2_PORT, tx, 4);
uint8_t rx[4] = {0};
int n = uart_read_bytes(U2_PORT, rx, 4, pdMS_TO_TICKS(20));
uint8_t chk = rx[0]^rx[1]^rx[2];
xSemaphoreTake(logmux, portMAX_DELAY);
if (n==4 && rx[0]==HDR && rx[3]==chk)
ESP_LOGI(TAG, "TLM tick=%d ok", rx[1]);
else
ESP_LOGW(TAG, "TLM bad n=%d", n);
xSemaphoreGive(logmux);
}
}

static void heartbeat_task(void *arg)
{
gpio_set_direction(LED_HB, GPIO_MODE_OUTPUT);
bool s = false;
while (1) {
s = !s; gpio_set_level(LED_HB, s);
xSemaphoreTake(logmux, portMAX_DELAY);
ESP_LOGI(TAG, "HB alive");
xSemaphoreGive(logmux);
vTaskDelay(pdMS_TO_TICKS(1000));
}
}
void app_main(void)
{
q_led = xQueueCreate(8, sizeof(lg_msg_t));
q_tlm = xQueueCreate(8, sizeof(lg_msg_t));
logmux = xSemaphoreCreateMutex();
xTaskCreate(model_task, "model", 3072, NULL, 3, NULL);
xTaskCreate(led_task, "led", 3072, NULL, 2, NULL);
xTaskCreate(telemetry_task, "tlm", 3072, NULL, 2, NULL);
xTaskCreate(heartbeat_task, "hb", 2560, NULL, 1, NULL);
}