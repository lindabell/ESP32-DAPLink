#include "ui_custom.h"
#include "assert.h"

void ui_custom_load_screen(lv_obj_t **newScreen, void (*ui_init)(void),
                           void (*event)(lv_event_t *e), lv_obj_t **oldScreen)
{
    assert(newScreen);
    assert(ui_init);
    assert(oldScreen);

    lv_obj_t *act_scr = lv_scr_act();
    assert(act_scr == (*oldScreen));

    if (*newScreen == NULL) ui_init();

    if (event) lv_obj_add_event_cb(*newScreen, event, LV_EVENT_ALL, NULL);
    lv_scr_load_anim(*newScreen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 0, 0, false);

    lv_group_t *group = lv_group_get_default();
    if (group) lv_group_add_obj(group, *newScreen);

    if (oldScreen && (*oldScreen))
    {
        lv_obj_del(*oldScreen);
        *oldScreen = NULL;
    }
}