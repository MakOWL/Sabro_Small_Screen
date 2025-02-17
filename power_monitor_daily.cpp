#include "side_menu.h"
#include "widget_dec.h"
#include <Arduino.h>
#include "charts.h"
#include <cmath>
#include "com_structs.h"

#include "images.h"

lv_coord_t power_monitor_hourly_units_array[HOURLY_ARRAY_SIZE + 2];
lv_coord_t power_monitor_daily_units_array[DAILY_ARRAY_SIZE + 2];
lv_coord_t power_monitor_monthly_cost_array[MONTHLY_ARRAY_SIZE + 2];

lv_coord_t power_monitor_hourly_cost_array[HOURLY_ARRAY_SIZE + 2];
lv_coord_t power_monitor_daily_cost_array[DAILY_ARRAY_SIZE + 2];
lv_coord_t power_monitor_monthly_units_array[MONTHLY_ARRAY_SIZE + 2];

lv_chart_series_t *units_series;
lv_chart_series_t *cost_series;

float esp_now_daily_off_peak_units_array[DAILY_ARRAY_SIZE];
float esp_now_daily_peak_units_array[DAILY_ARRAY_SIZE];

lv_obj_t *power_monitor_daily_screen;
lv_coord_t daily_unit_array[31] = {0};
lv_coord_t daily_cost_array[31] = {0};
lv_obj_t *daily_chart;
int peak_hour_cost = 60;
int off_peak_hour_cost = 55;

void update_daily_chart() {
  Serial.println("Updated daily chart, the function called atleast");
  int32_t maximum_unit = 0;
  int32_t minimum_unit = 0;
  int32_t maximum_cost = 0;
  int32_t minimum_cost = 0;

  
  for (uint32_t i = 0; i < DAILY_ARRAY_SIZE; i++) {
 
    if (isnan(esp_now_daily_off_peak_units_array[i]) ||
        isnan(esp_now_daily_peak_units_array[i])) {
      power_monitor_daily_units_array[i] = CHART_DATA_VALUE_IS_NAN;
      power_monitor_daily_cost_array[i] = CHART_DATA_VALUE_IS_NAN;
      continue;
    }

    power_monitor_daily_units_array[i] =
        (lv_coord_t)((esp_now_daily_off_peak_units_array[i] +
                      esp_now_daily_peak_units_array[i]) *
                     (int32_t)pow(10.0, (double) CHART_DATA_DECIMAL_POINT_PERCISION));

    power_monitor_daily_cost_array[i] =
        (lv_coord_t)(((esp_now_daily_off_peak_units_array[i] *
                       off_peak_hour_cost) +
                      (esp_now_daily_peak_units_array[i] * peak_hour_cost)) *
                     (int32_t)pow(10.0, (double)  CHART_DATA_DECIMAL_POINT_PERCISION));


    // ** New Feature: Read Data from running_data for Current Day **
    if (i == (data.rtc_day - 1)) {
      if (isnan(data.day_off_peak_energy) || isnan(data.day_peak_energy)) {
        power_monitor_daily_units_array[i] = CHART_DATA_VALUE_IS_NAN;
        power_monitor_daily_cost_array[i] = CHART_DATA_VALUE_IS_NAN;
        continue;
      }
      power_monitor_daily_units_array[i] =
          (lv_coord_t)((data.day_off_peak_energy +
                        data.day_peak_energy) *
                       (int32_t)pow(10.0, (double)  CHART_DATA_DECIMAL_POINT_PERCISION));

      power_monitor_daily_cost_array[i] =
          (lv_coord_t)(((data.day_off_peak_energy *
                         off_peak_hour_cost) +
                        (data.day_peak_energy * peak_hour_cost)) *
                       (int32_t)pow(10.0, (double)  CHART_DATA_DECIMAL_POINT_PERCISION));
    }

    // ** Find Maximum and Minimum Values for Chart Scaling **
    if (i == 0) {
      maximum_unit = power_monitor_daily_units_array[i];
      minimum_unit = power_monitor_daily_units_array[i];
      maximum_cost = power_monitor_daily_cost_array[i];
      minimum_cost = power_monitor_daily_cost_array[i];
    } else {
      maximum_unit = (power_monitor_daily_units_array[i] > maximum_unit) 
                       ? power_monitor_daily_units_array[i] 
                       : maximum_unit;

      minimum_unit = (power_monitor_daily_units_array[i] < minimum_unit) 
                       ? power_monitor_daily_units_array[i] 
                       : minimum_unit;

      maximum_cost = (power_monitor_daily_cost_array[i] > maximum_cost) 
                       ? power_monitor_daily_cost_array[i] 
                       : maximum_cost;

      minimum_cost = (power_monitor_daily_cost_array[i] < minimum_cost) 
                       ? power_monitor_daily_cost_array[i] 
                       : minimum_cost;
    }
  }

  // ** Adjust for Better Visualization **
  maximum_unit += ceil(maximum_unit * 0.10);
  maximum_cost += ceil(maximum_cost * 0.10);

  power_monitor_daily_units_array[DAILY_ARRAY_SIZE] = minimum_unit;
  power_monitor_daily_cost_array[DAILY_ARRAY_SIZE] = minimum_cost;
  power_monitor_daily_units_array[DAILY_ARRAY_SIZE + 1] = maximum_unit;
  power_monitor_daily_cost_array[DAILY_ARRAY_SIZE + 1] = maximum_cost;
 /* for (uint32_t i = 0; i < DAILY_ARRAY_SIZE; i++) {
  Serial.print("Units[");
  Serial.print(i);
  Serial.print("]: ");
  Serial.println(power_monitor_daily_units_array[i]);
  Serial.print("Cost[");
  Serial.print(i);
  Serial.print("]: ");
  Serial.println(power_monitor_daily_cost_array[i]);
}*/
  
  // ** Update Chart Ranges **
  if (daily_chart != NULL) {
    lv_chart_set_ext_y_array(daily_chart, units_series, power_monitor_daily_units_array);
    lv_chart_set_ext_y_array(daily_chart, cost_series, power_monitor_daily_cost_array);
   lv_chart_set_range(daily_chart, LV_CHART_AXIS_PRIMARY_Y,
                   power_monitor_daily_units_array[DAILY_ARRAY_SIZE],
                   power_monitor_daily_units_array[DAILY_ARRAY_SIZE + 1]);

lv_chart_set_range(daily_chart, LV_CHART_AXIS_SECONDARY_Y,
                   power_monitor_daily_cost_array[DAILY_ARRAY_SIZE],
                   power_monitor_daily_cost_array[DAILY_ARRAY_SIZE + 1]);
    lv_chart_refresh(daily_chart);
  }
}

void create_screen_power_monitor_daily_screen() {
     if (power_monitor_daily_screen != NULL) {
        lv_scr_load(power_monitor_daily_screen); // Load the existing screen
        return;
    }
    power_monitor_daily_screen = lv_obj_create(0);
    lv_obj_set_pos(power_monitor_daily_screen, 0, 0);
    lv_obj_set_size(power_monitor_daily_screen, 240, 320);
    {
        lv_obj_t *parent_obj = power_monitor_daily_screen;
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
      
        {
          daily_chart = lv_chart_create(parent_obj);
          lv_obj_set_pos(daily_chart, 0, 0);
          lv_obj_set_size(daily_chart, 207, 247);
          lv_obj_set_style_align(daily_chart, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_chart_set_type(daily_chart, LV_CHART_TYPE_LINE); // Change to BAR
          lv_chart_set_point_count(daily_chart, DAILY_ARRAY_SIZE);
          lv_chart_set_zoom_x(daily_chart, 1000);
          lv_chart_set_range(daily_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
          lv_chart_set_axis_tick(daily_chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 31, 2, true, 50);
          lv_chart_set_axis_tick(daily_chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, false, 50);
          lv_chart_set_axis_tick(daily_chart, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 5, 2, false, 25);
          //lv_obj_set_style_width(daily_chart, 15, LV_PART_ITEMS); // Set Bar Width
          lv_obj_set_style_bg_color(daily_chart, lv_color_hex(0xCFE3E8), LV_PART_ITEMS);

          units_series = lv_chart_add_series(daily_chart, lv_color_hex(0xCFE3E8), LV_CHART_AXIS_PRIMARY_Y);
          cost_series = lv_chart_add_series(daily_chart, lv_color_hex(0x1960EC), LV_CHART_AXIS_SECONDARY_Y);
        }
        }
    }
   // update_daily_chart(); 
}