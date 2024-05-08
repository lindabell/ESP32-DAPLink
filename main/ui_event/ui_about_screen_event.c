#include "ui_about_screen_event.h"
#include "ui_menu_screen_event.h"
#include "rts_key.h"
#include "ui_logo_screen_event.h"
#include "esp_log.h"

static void lv_ui_time_update(lv_timer_t *tmr)
{
    //ui_custom_load_screen(&ui_menu_screen, ui_menu_screen_screen_init,
    //                      menu_screen_event, &ui_logo_screen);
}

void about_screen_event(lv_event_t *e)
{
    static lv_timer_t *timer = NULL;
    uint32_t key = 0;

    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    switch (event_code)
    {
    case LV_EVENT_SCREEN_LOADED:
        timer = lv_timer_create(lv_ui_time_update, 1000, 0);
        assert(timer);
        break;
    case LV_EVENT_SCREEN_UNLOADED:
        if (timer)
        {
            lv_timer_del(timer);
            timer = NULL;
        }
        break;
    default:
        break;
    }

    if (event_code == LV_EVENT_KEY)
    {
        key = lv_event_get_key(e);
        ESP_LOGI("about","KEY EVENT:%lu",key);
        switch (key)
        {
        case RTS_KEY_EVENT_PRESSED:
            break;
        case RTS_KEY_EVENT_LONG_CLICK:
            ui_custom_load_screen(&ui_logo_screen, ui_logo_screen_screen_init,
                                  logo_screen_event, &ui_about_screen);
            break;
        default:
            break;
        }
    }
}