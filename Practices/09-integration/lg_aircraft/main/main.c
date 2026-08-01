#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_task_wdt.h"
#include "esp_log.h"
#include "operator0_module0.h"    

#define LED_RED 25
#define LED_GRN 26
#define LED_WHT 33
static const char *TAG = "AC";
static const int SERVO_GPIO[5] = { 13, 12, 14, 27, 4 }; 

typedef struct { int s[5]; } cmd_t;      
typedef struct { int mode, dir; } in_t;  

static QueueHandle_t q_act;
static SemaphoreHandle_t logmux;
static volatile in_t pilot = {0,0};
static volatile int  g_phase = 0;
static volatile bool g_emerg = false;

static void servos_init(void){
    ledc_timer_config_t t={.speed_mode=LEDC_LOW_SPEED_MODE,.timer_num=LEDC_TIMER_0,
        .duty_resolution=LEDC_TIMER_16_BIT,.freq_hz=50,.clk_cfg=LEDC_AUTO_CLK};
    ledc_timer_config(&t);
    for(int i=0;i<5;i++){ ledc_channel_config_t c={.gpio_num=SERVO_GPIO[i],
        .speed_mode=LEDC_LOW_SPEED_MODE,.channel=i,.timer_sel=LEDC_TIMER_0,
        .duty=0,.hpoint=0}; ledc_channel_config(&c); }
}
static void servo_write(int ch,int deg){
    if(deg<0){deg=0;} if(deg>180){deg=180;}
    int us=500+(2500-500)*deg/180;
    uint32_t d=(uint32_t)((uint64_t)us*65535/20000);
    ledc_set_duty(LEDC_LOW_SPEED_MODE,ch,d);
    ledc_update_duty(LEDC_LOW_SPEED_MODE,ch);
}

static void sensor_task(void *arg){
    adc_oneshot_unit_handle_t adc;
    adc_oneshot_unit_init_cfg_t u={.unit_id=ADC_UNIT_1}; adc_oneshot_new_unit(&u,&adc);
    adc_oneshot_chan_cfg_t ch={.atten=ADC_ATTEN_DB_12,.bitwidth=ADC_BITWIDTH_DEFAULT};
    adc_oneshot_config_channel(adc,ADC_CHANNEL_6,&ch);   // GPIO34 = mode
    adc_oneshot_config_channel(adc,ADC_CHANNEL_7,&ch);   // GPIO35 = dir
    while(1){ int m,d;
        adc_oneshot_read(adc,ADC_CHANNEL_6,&m);
        adc_oneshot_read(adc,ADC_CHANNEL_7,&d);
        pilot.mode=m; pilot.dir=d;
        vTaskDelay(pdMS_TO_TICKS(50)); }
}

static void model_task(void *arg){
    gpio_set_direction(LED_RED,GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_GRN,GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_WHT,GPIO_MODE_OUTPUT);
    outC_operator0_module0 ctx; operator0_reset_module0(&ctx);
    esp_task_wdt_add(NULL);
    TickType_t last=xTaskGetTickCount(); bool blink=false;
    while(1){
        esp_task_wdt_reset();
        swan_bool despegue,tierra,cruise,aterrizaje,red,green,blanca,emergency,safe;
        swan_int32 aleizq,aleder,tdir,elev,dirf,cont,emergency_off,val,lgr;
        operator0_module0(pilot.mode, pilot.dir,
            &despegue,&tierra,&cruise,&aterrizaje,
            &aleizq,&aleder,&tdir,&elev,
            &red,&green,&blanca,&dirf,&emergency,&cont,&emergency_off,
            &safe,&val,&lgr,&ctx);
        cmd_t cmd = { .s = { (int)aleizq,(int)aleder,(int)tdir,(int)elev,(int)lgr } };
        xQueueSend(q_act,&cmd,0);
        gpio_set_level(LED_GRN, green);
        gpio_set_level(LED_WHT, blanca);
        if(emergency){ blink=!blink; gpio_set_level(LED_RED,blink); }
        else         { gpio_set_level(LED_RED, red); }
        g_phase = tierra?0:despegue?1:cruise?2:aterrizaje?3:0;
        g_emerg = emergency;
        vTaskDelayUntil(&last,pdMS_TO_TICKS(100));
    }
}

static void actuator_task(void *arg){
    servos_init(); cmd_t c;
    while(1) if(xQueueReceive(q_act,&c,portMAX_DELAY)==pdTRUE)
        for(int i=0;i<5;i++) servo_write(i,c.s[i]);
}

static void telemetry_task(void *arg){
    const char *ph[]={"GROUND","TAKEOFF","CRUISE","LANDING"};
    int last=-1; bool le=false;
    while(1){
        if(g_phase!=last || g_emerg!=le){
            xSemaphoreTake(logmux,portMAX_DELAY);
            ESP_LOGW(TAG,"phase=%s emergency=%d",ph[g_phase],(int)g_emerg);
            xSemaphoreGive(logmux); last=g_phase; le=g_emerg;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void){
    q_act  = xQueueCreate(4,sizeof(cmd_t));
    logmux = xSemaphoreCreateMutex();
    xTaskCreate(model_task,    "model",4096,NULL,4,NULL);
    xTaskCreate(actuator_task, "act",  3072,NULL,3,NULL);
    xTaskCreate(sensor_task,   "sens", 3072,NULL,3,NULL);
    xTaskCreate(telemetry_task,"tlm",  3072,NULL,2,NULL);
}