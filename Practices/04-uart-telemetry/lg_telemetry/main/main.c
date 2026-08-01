#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "operator1_module0.h" 
#define U1_PORT UART_NUM_2
#define U1_TX 4
#define U1_RX 5
#define HDR 0xAA
#define LED_RED 25
#define LED_YEL 26
#define LED_GRN 27
static const char *TAG = "TLM";
static outC_operator1_module0 ctx;
static swan_int32 cont;
static swan_bool green, yellow, red;
void app_main(void)
{

uart_config_t cfg = {
.baud_rate = 115200, .data_bits = UART_DATA_8_BITS,
.parity = UART_PARITY_DISABLE, .stop_bits = UART_STOP_BITS_1,
.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
.source_clk = UART_SCLK_DEFAULT,
};
uart_driver_install(U1_PORT, 256, 0, 0, NULL, 0);
uart_param_config(U1_PORT, &cfg);
uart_set_pin(U1_PORT, U1_TX, U1_RX,
UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
gpio_set_direction(LED_YEL, GPIO_MODE_OUTPUT);
gpio_set_direction(LED_GRN, GPIO_MODE_OUTPUT);
operator1_reset_module0(&ctx);
while (1) {
operator1_module0(&cont, &green, &yellow, &red, &ctx);
gpio_set_level(LED_RED, red);
gpio_set_level(LED_YEL, yellow);
gpio_set_level(LED_GRN, green);

uint8_t state = (green ? 1 : 0) | (yellow ? 2 : 0) | (red ? 4 : 0);
uint8_t tx[4];
tx[0] = HDR;
tx[1] = (uint8_t)cont;
tx[2] = state;
tx[3] = tx[0] ^ tx[1] ^ tx[2]; 

uart_write_bytes(U1_PORT, tx, 4);
uint8_t rx[4] = {0};
int n = uart_read_bytes(U1_PORT, rx, 4, pdMS_TO_TICKS(20));

uint8_t chk = rx[0] ^ rx[1] ^ rx[2];
if (n == 4 && rx[0] == HDR && rx[3] == chk) {
ESP_LOGI(TAG, "TX %02X %02X %02X %02X | RX tick=%d G=%d Y=%d R=%d",
tx[0], tx[1], tx[2], tx[3], rx[1],
rx[2] & 1, (rx[2] >> 1) & 1, (rx[2] >> 2) & 1);
} else {
ESP_LOGW(TAG, "RX-BAD n=%d hdr=%02X chk=%02X/%02X", n, rx[0], rx[3], chk);
}
vTaskDelay(pdMS_TO_TICKS(100));
}
}