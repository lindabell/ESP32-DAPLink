#include "user_lvgl_port.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lvgl_port.h"

#include "esp_lvgl_port_indev.h"
#include "ui.h"
#include "ui_custom.h"
#include "ui_logo_screen_event.h"
#include "task_key.h"

/* LCD size */
#define EXAMPLE_LCD_H_RES   (240)
#define EXAMPLE_LCD_V_RES   (135)

/* LCD settings */
#define EXAMPLE_LCD_SPI_NUM         (SPI3_HOST)
#define EXAMPLE_LCD_PIXEL_CLK_HZ    (80 * 1000 * 1000)
#define EXAMPLE_LCD_CMD_BITS        (8)
#define EXAMPLE_LCD_PARAM_BITS      (8)
#define EXAMPLE_LCD_COLOR_SPACE     (ESP_LCD_COLOR_SPACE_RGB)
#define EXAMPLE_LCD_BITS_PER_PIXEL  (16)
#define EXAMPLE_LCD_DRAW_BUFF_DOUBLE (1)
#define EXAMPLE_LCD_DRAW_BUFF_HEIGHT (50)
#define EXAMPLE_LCD_BL_ON_LEVEL     (1)

/* LCD pins */
#define EXAMPLE_LCD_GPIO_SCLK       (GPIO_NUM_12)
#define EXAMPLE_LCD_GPIO_MOSI       (GPIO_NUM_11)
#define EXAMPLE_LCD_GPIO_RST        (GPIO_NUM_9)
#define EXAMPLE_LCD_GPIO_DC         (GPIO_NUM_8)
#define EXAMPLE_LCD_GPIO_CS         (GPIO_NUM_10)
#define EXAMPLE_LCD_GPIO_BL         (GPIO_NUM_7)

static const char *TAG = "lvgl_port";

/* LCD IO and panel */
static esp_lcd_panel_io_handle_t lcd_io = NULL;
static esp_lcd_panel_handle_t lcd_panel = NULL;

/* LVGL display and touch */
static lv_display_t *lvgl_disp = NULL;

static esp_err_t app_lcd_init(void)
{
    esp_err_t ret = ESP_OK;

    /* LCD backlight */
    gpio_config_t bk_gpio_config = {
        .pin_bit_mask = 1ULL << EXAMPLE_LCD_GPIO_BL,
        .mode = GPIO_MODE_OUTPUT,
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    /* LCD initialization */
    ESP_LOGD(TAG, "Initialize SPI bus");
    const spi_bus_config_t buscfg = {
        .mosi_io_num = EXAMPLE_LCD_GPIO_MOSI,
        .miso_io_num = GPIO_NUM_NC,
        .sclk_io_num = EXAMPLE_LCD_GPIO_SCLK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .data4_io_num = GPIO_NUM_NC,
        .data5_io_num = GPIO_NUM_NC,
        .data6_io_num = GPIO_NUM_NC,
        .data7_io_num = GPIO_NUM_NC,
        .max_transfer_sz =
            EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DRAW_BUFF_HEIGHT * sizeof(uint16_t),
    };
    ESP_RETURN_ON_ERROR(spi_bus_initialize(EXAMPLE_LCD_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO), TAG, "SPI init failed");

    ESP_LOGD(TAG, "Install panel IO");
    const esp_lcd_panel_io_spi_config_t io_config = {
        .cs_gpio_num = EXAMPLE_LCD_GPIO_CS,
        .dc_gpio_num = EXAMPLE_LCD_GPIO_DC,
        .spi_mode = 0,
        .pclk_hz = EXAMPLE_LCD_PIXEL_CLK_HZ,
        .trans_queue_depth = 10,

        .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
        .lcd_param_bits = EXAMPLE_LCD_PARAM_BITS,
        
        
    };
    //ESP_GOTO_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)EXAMPLE_LCD_SPI_NUM, &io_config, &lcd_io), err, TAG, "New panel IO failed");
    esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)EXAMPLE_LCD_SPI_NUM, &io_config, &lcd_io);

    ESP_LOGD(TAG, "Install LCD driver");
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_LCD_GPIO_RST,
        .color_space = (lcd_rgb_element_order_t)EXAMPLE_LCD_COLOR_SPACE,
        .bits_per_pixel = EXAMPLE_LCD_BITS_PER_PIXEL,
    };
    //ESP_GOTO_ON_ERROR(esp_lcd_new_panel_st7789(lcd_io, &panel_config, &lcd_panel), err, TAG, "New panel failed");
    esp_lcd_new_panel_st7789(lcd_io, &panel_config, &lcd_panel);

    esp_lcd_panel_reset(lcd_panel);
    esp_lcd_panel_init(lcd_panel);
    esp_lcd_panel_set_gap(lcd_panel,40,53);
    esp_lcd_panel_invert_color(lcd_panel,true);
    esp_lcd_panel_swap_xy(lcd_panel,true);
    esp_lcd_panel_mirror(lcd_panel, true, false);
    esp_lcd_panel_disp_on_off(lcd_panel, true);

    /* LCD backlight on */
    ESP_ERROR_CHECK(gpio_set_level(EXAMPLE_LCD_GPIO_BL, EXAMPLE_LCD_BL_ON_LEVEL));

    return ret;
//err:
    if (lcd_panel) {
        esp_lcd_panel_del(lcd_panel);
    }
    if (lcd_io) {
        esp_lcd_panel_io_del(lcd_io);
    }
    spi_bus_free(EXAMPLE_LCD_SPI_NUM);
    return ret;
}

static esp_err_t app_lvgl_init(void)
{
    /* Initialize LVGL */
    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 4,         /* LVGL task priority */
        .task_stack = 4096,         /* LVGL task stack size */
        .task_affinity = -1,        /* LVGL task pinned to core (-1 is no affinity) */
        .task_max_sleep_ms = 500,   /* Maximum sleep in LVGL task */
        .timer_period_ms = 5        /* LVGL timer tick period in ms */
    };
    ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "LVGL port initialization failed");

    /* Add LCD screen */
    ESP_LOGD(TAG, "Add LCD screen");
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = lcd_io,
        .panel_handle = lcd_panel,
        .buffer_size = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DRAW_BUFF_HEIGHT * sizeof(uint16_t),
        .double_buffer = EXAMPLE_LCD_DRAW_BUFF_DOUBLE,
        .hres = EXAMPLE_LCD_H_RES,
        .vres = EXAMPLE_LCD_V_RES,
        .monochrome = false,
        /* Rotation values must be same as used in esp_lcd for initial settings of the screen */
        .rotation = {
            .swap_xy = true,
            .mirror_x = true,
            .mirror_y = false,
        },
        .flags = {
            .buff_dma = true,
            .swap_bytes = true,
        }
    };
    lvgl_disp = lvgl_port_add_disp(&disp_cfg);

    return ESP_OK;
}

void user_lvgl_port_init(void)
{
/* LCD HW initialization */
    ESP_ERROR_CHECK(app_lcd_init());

    /* LVGL initialization */
    ESP_ERROR_CHECK(app_lvgl_init());

    /* keypad init*/
    esp_lvgl_port_indev_init();

    /* creat key task*/
    task_key_create();

    /* setup ui*/
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(
        dispp, lv_palette_main(LV_PALETTE_BLUE),
        lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);

    lv_disp_set_theme(dispp, theme);

    lv_obj_t * screeen=lv_scr_act();

    ui_custom_load_screen(&ui_logo_screen,ui_logo_screen_screen_init,logo_screen_event,&screeen);
}