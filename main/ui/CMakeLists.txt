SET(SOURCES screens/ui_logo_screen.c
    screens/ui_menu_screen.c
    screens/ui_about_screen.c
    screens/ui_offlink_setting_screen.c
    screens/ui_offlink_download_screen.c
    screens/ui_usb_uart_screen.c
    screens/ui_daplink_screen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    images/ui_img_image_logo_png.c
    images/ui_img_image_tree_png.c
    fonts/ui_font_RobotoLight93.c
    fonts/ui_font_RobotoMedium22.c
    fonts/ui_font_RobotoMedium28.c
    fonts/ui_font_RobotoRegular16.c
    fonts/ui_font_RobotoRegular38.c
    fonts/ui_font_RobotoRegular58.c
    fonts/ui_font_RobotoRegular65.c)

add_library(ui ${SOURCES})
