
#ifndef _ESP_LVGL_PORT_INDEV_H
#define _ESP_LVGL_PORT_INDEV_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

    enum
    {
        KEY_NONE = 0,
        KEY_PRESS_DOWN = 112,       // F1
        KEY_PRESS_UP = 113,         // F2
        KEY_PRESS_REPEAT = 114,     // F3
        KEY_SINGLE_CLICK = 115,     // F4
        KEY_DOUBLE_CLICK = 116,     // F5
        KEY_LONG_PRESS_START = 117, // F6
        KEY_LONG_PRESS_HOLD = 118,  // F7
        KEY_MAX
    };

    void esp_lvgl_port_indev_init(void);
    void esp_lvgl_port_indev_queue_key(uint32_t key);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
