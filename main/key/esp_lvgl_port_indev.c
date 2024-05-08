#include "esp_lvgl_port_indev.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "lvgl.h"
#include "ui.h"

static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

static lv_indev_t *indev_keypad=NULL;
static lv_group_t *group = NULL;
static QueueHandle_t queue_handle = NULL;

void esp_lvgl_port_indev_init(void)
{
    static lv_indev_drv_t indev_drv;

    queue_handle = xQueueCreate(10, sizeof(uint32_t));
    assert(queue_handle);

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keypad_read;
    indev_keypad = lv_indev_drv_register(&indev_drv);

    group = lv_group_create();
    lv_indev_set_group(indev_keypad, group);
    lv_group_set_default(group);
}

/*Will be called by the library to read the mouse*/
static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static uint32_t key = 0;
    static uint32_t last_key = 0;

    if (pdTRUE == xQueueReceive(queue_handle, (void *)&key, 0))
    {
        data->state = LV_INDEV_STATE_PR;
        last_key = key;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
    data->key = last_key;
}

void esp_lvgl_port_indev_queue_key(uint32_t key)
{
    xQueueSend(queue_handle, (void *)&key, 0);
}
