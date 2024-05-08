#include "ui_daplink_screen_event.h"
#include "ui_menu_screen_event.h"
#include "rts_key.h"
#include "ui_logo_screen_event.h"
#include "esp_log.h"

void daplink_screen_event(lv_event_t *e)
{
    static lv_timer_t *timer = NULL;
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
        ESP_LOGI("daplink","KEY EVENT:%lu",key);
        switch (key)
        {
        case RTS_KEY_EVENT_PRESSED:
            break;
        case RTS_KEY_EVENT_LONG_CLICK:
            ui_custom_load_screen(&ui_logo_screen, ui_logo_screen_screen_init,
                                  logo_screen_event, &ui_daplink_screen);
            break;
        default:
            break;
        }
    }
}