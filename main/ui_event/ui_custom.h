#ifndef _UI_CUSTOM_H_
#define _UI_CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "ui.h"

void ui_custom_load_screen(lv_obj_t **newScreen, void (*ui_init)(void),
                           void (*event)(lv_event_t *e), lv_obj_t **oldScreen);

#ifdef __cplusplus
}
#endif
#endif