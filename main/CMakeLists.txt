file(GLOB_RECURSE SRC_UI ${CMAKE_SOURCE_DIR} "ui/*.c")
file(GLOB_RECURSE SRC_UI_EVENT ${CMAKE_SOURCE_DIR} "ui_event/*.c")
idf_component_register(
    SRCS ${SRC_UI}
	SRCS ${SRC_UI_EVENT}    
    SRCS "main.cpp" 
    "cdc_uart.c"
    "msc_disk.c"
    "web_handler.cpp"
    "usb_cdc_handler.c"
    "web_server.c"
    "usb_desc.c"
    "prog.cpp"
    "programmer.cpp"
    "tcp_netconn.c"
    "tcp_server.c"
    "usbip_server.c"
    "kcp_server.c"
    "DAP_handle.c"
    "prog_data.cpp"
    "prog_idle.cpp"
    "prog_online.cpp"
    "prog_offline.cpp"
    "lvgl_port/user_lvgl_port.c"
    "key/esp_lvgl_port_indev.c"
	"key/task_key.c"
	"key/rts_key/rts_key.c"
    INCLUDE_DIRS .
    "ui"
    "ui_event"
    "key"
	"key/rts_key"
    "lvgl_port/"
    EMBED_FILES "../html/root.html"
    "../html/favicon.ico"
    "../html/program.html"
    "../html/webserial.html")

    add_compile_definitions(RTS_KEY_ENABLE=1 RTS_KEY_DOUBEL_CLICK_ENABLE=1 RTS_KEY_SHORT_CNT_MIN=2)