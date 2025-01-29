#include "Panelz.h"
#include "widget_dec.h"
#include "lvgl.h"

Panelz tft(BOARD_SC05_PLUS);

static const uint16_t screenWidth  = 240;
static const uint16_t screenHeight = 320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[2][ screenWidth * 10 ];


//objects_t objects;


/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    if (tft.getStartCount() == 0)
    {   // Processing if not yet started
        tft.startWrite();
    }
    tft.pushImageDMA( area->x1
                    , area->y1
                    , area->x2 - area->x1 + 1
                    , area->y2 - area->y1 + 1
                    , ( lgfx::swap565_t* )&color_p->full);
    lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;

    data->state = LV_INDEV_STATE_REL;

    if( tft.getTouch( &touchX, &touchY ) )
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

#include "lvgl.h"

/*void swap_red_blue_screen(void) {
     Get the display buffer 
    lv_disp_t *disp = lv_disp_get_default();
    lv_draw_buf_t *draw_buf = lv_disp_get_draw_buf(disp);
    lv_color_t *buffer = draw_buf->buf1;  // Use buffer1 or buffer2 depending on your setup

    /* Get the width and height of the screen 
    uint32_t screen_width = lv_disp_get_hor_res(disp);
    uint32_t screen_height = lv_disp_get_ver_res(disp);

    lv_color_t pixel;
    uint8_t r, g, b;
    lv_color_t swapped_pixel;

    /* Iterate over each pixel of the screen 
    for (uint32_t y = 0; y < screen_height; y++) {
        for (uint32_t x = 0; x < screen_width; x++) {
            pixel = buffer[y * screen_width + x];

            /* Extract RGB components from the pixel 
            r = lv_color_get_red(pixel);
            g = lv_color_get_green(pixel);
            b = lv_color_get_blue(pixel);

            /* Swap red and blue 
            swapped_pixel = lv_color_make(b, g, r);

            /* Update the pixel with the swapped color 
            buffer[y * screen_width + x] = swapped_pixel;
        }
    }

    /* Force a refresh to update the display 
    lv_disp_flush_ready(disp);
}*/


void setup()
{
    tft.begin();

    lv_init();
    lv_disp_draw_buf_init( &draw_buf, buf[0], buf[1], screenWidth * 10 );
    //ui_init();

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );
    tft.fillScreen(TFT_PURPLE);
    //swap_red_blue_screen();

    //lvgl_ui();
    create_main_screen();
}


void loop()
{
  Serial.begin(115200);
  lv_timer_handler(); /* let the GUI do its work */
  delay(20);
}
