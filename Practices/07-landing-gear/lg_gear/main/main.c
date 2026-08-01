#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "FlightPhase_module0.h" 
#define BTN 0
#define LED_GRN 25
#define LED_YEL 26
#define LED_RED 27
#define SERVO_GPIO 13
#define POT_CH ADC_CHANNEL_6 
static const char *TAG = "GEAR";
static QueueHandle_t q_evt; 
static QueueHandle_t q_act; 
static SemaphoreHandle_t logmux;
static volatile int gear_cmd = 0; 

static void servo_init(void){ ledc_timer_config_t t={.speed_mode=LEDC_LOW_SPEED_MODE,
.timer_num=LEDC_TIMER_0,.duty_resolution=LEDC_TIMER_16_BIT,.freq_hz=50,
.clk_cfg=LEDC_AUTO_CLK}; ledc_timer_config(&t);
ledc_channel_config_t c={.gpio_num=SERVO_GPIO,.speed_mode=LEDC_LOW_SPEED_MODE,
.channel=LEDC_CHANNEL_0,.timer_sel=LEDC_TIMER_0,.duty=0,.hpoint=0};
ledc_channel_config(&c); }
static void servo_write(int deg){ if(deg<0)deg=0;
if(deg>180)deg=180;
int us=500+(2500-500)*deg/180;
uint32_t d=(uint32_t)((uint64_t)us*65535/20000);
ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,d);
ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0); }

static void input_task(void *arg){
gpio_config_t io={.pin_bit_mask=1ULL<<BTN,.mode=GPIO_MODE_INPUT,
.pull_up_en=GPIO_PULLUP_ENABLE}; gpio_config(&io);
int prev=1;
while(1){ int now=gpio_get_level(BTN);
if(prev==1&&now==0){ uint8_t e=1; xQueueSend(q_evt,&e,0);}
prev=now; vTaskDelay(pdMS_TO_TICKS(20)); }
}

static void model_task(void *arg){
gpio_set_direction(LED_GRN,GPIO_MODE_OUTPUT);
gpio_set_direction(LED_YEL,GPIO_MODE_OUTPUT);
gpio_set_direction(LED_RED,GPIO_MODE_OUTPUT);
outC_FlightPhase_module0 ctx; swan_int32 phase;
FlightPhase_reset_module0(&ctx);
const int gear_of[4]={0,45,90,0}; 
TickType_t last=xTaskGetTickCount(); int last_p=-1;
while(1){
uint8_t e; swan_bool next=false;
if(xQueueReceive(q_evt,&e,0)==pdTRUE) next=true;
FlightPhase_module0(next,&phase,&ctx);
int p=(int)phase;
gpio_set_level(LED_GRN,(p==0||p==1));
gpio_set_level(LED_YEL,(p==1||p==2||p==3));
gpio_set_level(LED_RED,(p==3));
int cmd=gear_of[p]; gear_cmd=cmd;
xQueueSend(q_act,&cmd,0);
if(p!=last_p){ const char *n[]={"GROUND","TAKEOFF","CRUISE","LANDING"};
xSemaphoreTake(logmux,portMAX_DELAY);
ESP_LOGI(TAG,"phase=%s gear_cmd=%d deg",n[p],cmd);
xSemaphoreGive(logmux); last_p=p; }
vTaskDelayUntil(&last,pdMS_TO_TICKS(100));
}
}

static void actuator_task(void *arg){
servo_init(); int cmd;
while(1) if(xQueueReceive(q_act,&cmd,portMAX_DELAY)==pdTRUE) servo_write(cmd);
}

static void sensor_task(void *arg){
adc_oneshot_unit_handle_t adc;
adc_oneshot_unit_init_cfg_t u={.unit_id=ADC_UNIT_1}; adc_oneshot_new_unit(&u,&adc);
adc_oneshot_chan_cfg_t ch={.atten=ADC_ATTEN_DB_12,.bitwidth=ADC_BITWIDTH_DEFAULT};
adc_oneshot_config_channel(adc,POT_CH,&ch);
while(1){
int raw; adc_oneshot_read(adc,POT_CH,&raw);
int measured=raw*180/4095;
xSemaphoreTake(logmux,portMAX_DELAY);
ESP_LOGI(TAG,"gear cmd=%d measured=%d", gear_cmd, measured);
xSemaphoreGive(logmux);
vTaskDelay(pdMS_TO_TICKS(500));
}
}
void app_main(void){
q_evt = xQueueCreate(4,sizeof(uint8_t));
q_act = xQueueCreate(4,sizeof(int));
logmux = xSemaphoreCreateMutex();
xTaskCreate(input_task, "input",2048,NULL,2,NULL);
xTaskCreate(model_task, "model",3072,NULL,3,NULL);
xTaskCreate(actuator_task,"act", 3072,NULL,2,NULL);
xTaskCreate(sensor_task, "sens", 3072,NULL,1,NULL);
}