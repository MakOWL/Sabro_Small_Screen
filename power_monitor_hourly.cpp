#include <Arduino.h>
#include "charts.h"
#include <cmath>
#include "images.h"


void create_screen_power_monitor_hourly_screen(){
  if(power_monitor_hourly_screen != NULL){
     lv_scr_load(power_monitor_hourly_screen);
     return;
  }
    power_monitor_hourly_screen = lv_obj_create(0);
    lv_obj_set_pos(power_monitor_hourly_screen, 0, 0);
    lv_obj_set_size(power_monitor_hourly_screen, 240, 320);
    {
        lv_obj_t *parent_obj = power_monitor_hourly_screen;
        {
            lv_obj_t *menu_btn = lv_btn_create(parent_obj);
            lv_obj_set_pos(menu_btn, 10, 10);
            lv_obj_set_size(menu_btn, 33, 24);
            lv_obj_add_event_cb(menu_btn, menu_button_event_handler, LV_EVENT_CLICKED, NULL);
            lv_obj_set_style_align(menu_btn, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = menu_btn;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }    
            }
    hourly_chart = lv_chart_create(parent_obj);
    lv_obj_set_pos(hourly_chart, 0, 10);
    lv_obj_set_size(hourly_chart, 200, 250);
    lv_obj_set_style_align(hourly_chart, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_chart_set_type(hourly_chart, LV_CHART_TYPE_BAR);
    lv_chart_set_zoom_x(hourly_chart, 1000);
    lv_chart_set_point_count(hourly_chart, 24);
    lv_chart_set_div_line_count(hourly_chart, 50, 24);
    lv_chart_set_axis_tick(hourly_chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 24, 2, true, 50);
    lv_chart_set_axis_tick(hourly_chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, false, 50);
    lv_chart_set_axis_tick(hourly_chart, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 5, 2, false, 25);
    lv_obj_set_style_pad_gap(hourly_chart, -2, LV_PART_ITEMS | LV_STATE_DEFAULT);
    hourly_chart_units_series = lv_chart_add_series(hourly_chart, lv_color_hex(0xCFE3E8),LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(hourly_chart, hourly_chart_units_series, power_monitor_hourly_units_array);
    hourly_chart_cost_series = lv_chart_add_series(hourly_chart, lv_color_hex(0x1960EC),LV_CHART_AXIS_SECONDARY_Y);
    lv_chart_set_ext_y_array(hourly_chart, hourly_chart_cost_series, power_monitor_hourly_cost_array);
    lv_chart_set_range(hourly_chart, LV_CHART_AXIS_PRIMARY_Y,power_monitor_hourly_units_array[HOURLY_ARRAY_SIZE],power_monitor_hourly_units_array[HOURLY_ARRAY_SIZE + 1]);
    lv_chart_set_range(hourly_chart, LV_CHART_AXIS_SECONDARY_Y,power_monitor_hourly_cost_array[HOURLY_ARRAY_SIZE], power_monitor_hourly_cost_array[HOURLY_ARRAY_SIZE + 1]);
    lv_chart_refresh(hourly_chart);

    lv_chart_cursor_t *cursor_primary_y = lv_chart_add_cursor(hourly_chart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);
    static chart_event_data_t hourly_chart_event_data = { NULL, NULL, HOURLY_CHART };
    hourly_chart_event_data.cursor_primary_y = cursor_primary_y;
    lv_obj_add_event_cb(hourly_chart, chart_action, LV_EVENT_ALL, &hourly_chart_event_data);
    lv_obj_refresh_ext_draw_size(hourly_chart);

}
}