#include "freertos/FreeRTOS.h"
 #include "freertos/task.h"
 #include "esp_log.h"
 #include "operator0_module0.h" // generated
 static const char *TAG = "SCADE";
 static outC_operator0_module0 ctx; // generated context
 static swan_int32 cont; // model output
 void app_main(void)
 {ESP_LOGI(TAG, "scade_counter | first generated-code run");
 operator0_reset_module0(&ctx);
 while (1) {
 operator0_module0(&cont, &ctx); // one synchronous cycle
 ESP_LOGI(TAG, "count=%d", (int)cont);
 vTaskDelay(pdMS_TO_TICKS(100));
 }
 }