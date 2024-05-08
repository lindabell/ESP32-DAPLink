#include "ui_menu_screen_event.h"

#include "esp_log.h"
#include "rts_key.h"
#include "ui_about_screen_event.h"
#include "ui_logo_screen_event.h"
#include "ui_usb_uart_screen_event.h"

static void lv_ui_time_update(lv_timer_t *tmr)
{
    // ui_custom_load_screen(&ui_logo_screen, ui_logo_screen_screen_init,
    //                               logo_screen_event, &ui_menu_screen);
}

void menu_screen_event(lv_event_t *e)
{
    static lv_timer_t *timer = NULL;
    uint32_t key = 0;
    uint16_t id = 0;

    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    // ESP_LOGI("menu","event_code:%d",event_code);

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
        ESP_LOGI("menu", "KEY EVENT:%lu", key);
        switch (key)
        {
        case RTS_KEY_EVENT_CLICK:
            id = lv_roller_get_selected(ui_roller_menu);
            id++;
            if (id >= lv_roller_get_option_cnt(ui_roller_menu)) id = 0;
            lv_roller_set_selected(ui_roller_menu, id, LV_ANIM_ON);
            break;
        case RTS_KEY_EVENT_LONG_CLICK:
            char strBuf[32];
            lv_roller_get_selected_str(ui_roller_menu, strBuf, sizeof(strBuf));

            if (strcmp(strBuf, "About") == 0)
            {
                ui_custom_load_screen(&ui_about_screen, ui_about_screen_screen_init, about_screen_event, &ui_menu_screen);
            }
            else if (strcmp(strBuf, "USB To Uart") == 0)
            {
                ui_custom_load_screen(&ui_usb_uart_screen, ui_usb_uart_screen_screen_init, usb_uart_screen_event, &ui_menu_screen);
            }
            else
            {
                ui_custom_load_screen(&ui_logo_screen, ui_logo_screen_screen_init, logo_screen_event, &ui_menu_screen);
            }

            break;
        default:
            break;
        }
    }
}