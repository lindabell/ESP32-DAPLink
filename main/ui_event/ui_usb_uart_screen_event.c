#include "ui_usb_uart_screen_event.h"

#include "esp_log.h"
#include "rts_key.h"
#include "ui_logo_screen_event.h"
#include "ui_menu_screen_event.h"

void usb_uart_screen_event(lv_event_t *e)
{
    uint32_t key = 0;

    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    switch (event_code)
    {
    case LV_EVENT_SCREEN_LOADED:
        break;
    case LV_EVENT_SCREEN_UNLOADED:
        break;
    default:
        break;
    }

    if (event_code == LV_EVENT_KEY)
    {
        key = lv_event_get_key(e);
        ESP_LOGI("about", "KEY EVENT:%lu", key);
        switch (key)
        {
        case RTS_KEY_EVENT_PRESSED:
            break;
        case RTS_KEY_EVENT_LONG_CLICK:
            ui_custom_load_screen(&ui_logo_screen, ui_logo_screen_screen_init, logo_screen_event, &ui_usb_uart_screen);
            break;
        default:
            break;
        }
    }
}