#include "task_key.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "stdint.h"
#include "esp_lvgl_port_indev.h"
#include "rts_key.h"
#include "esp_log.h"

static bool readpin(void)
{
    if(gpio_get_level(GPIO_NUM_0))return false;
    else return true;
}

void task_key(void *pvParameters)
{
    static rts_key_t key;

    esp_lvgl_port_indev_init();

    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_NUM_0, GPIO_PULLUP_ONLY);

    rts_key_init();
    rts_key_pin_register(0,readpin);
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));

        key=rts_key_scan_10ms();
        if(key.event!=RTS_KEY_EVENT_NONE)
        {
            ESP_LOGI("key","key event=%d",key.event);
            esp_lvgl_port_indev_queue_key(key.event);
        }
    }
}

void task_key_create(void)
{
    xTaskCreate(task_key, "task_key", 10*1024, NULL, 12, NULL);
}