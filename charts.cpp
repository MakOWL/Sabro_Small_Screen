#include <Arduino.h>
#include "charts.h"
#include <cmath>

static const char *month_names[] = { "Jan", "Feb", "Mar", "Apr",
                                     "May", "Jun", "Jul", "Aug",
                                     "Sep", "Oct", "Nov", "Dec" };

lv_coord_t power_monitor_hourly_units_array[HOURLY_ARRAY_SIZE + 2];
lv_coord_t power_monitor_daily_units_array[DAILY_ARRAY_SIZE + 2];
lv_coord_t power_monitor_monthly_cost_array[MONTHLY_ARRAY_SIZE + 2];

lv_coord_t power_monitor_hourly_cost_array[HOURLY_ARRAY_SIZE + 2];
lv_coord_t power_monitor_daily_cost_array[DAILY_ARRAY_SIZE + 2];
lv_coord_t power_monitor_monthly_units_array[MONTHLY_ARRAY_SIZE + 2];

lv_chart_series_t *units_series;
lv_chart_series_t *cost_series;
lv_chart_series_t *hourly_chart_units_series;
lv_chart_series_t *hourly_chart_cost_series;

float esp_now_daily_off_peak_units_array[DAILY_ARRAY_SIZE];
float esp_now_daily_peak_units_array[DAILY_ARRAY_SIZE];

lv_obj_t *power_monitor_hourly_screen;
lv_obj_t *power_monitor_daily_screen;

lv_coord_t daily_unit_array[31] = {0};
lv_coord_t daily_cost_array[31] = {0};
lv_obj_t *daily_chart;
lv_obj_t *hourly_chart;
int peak_hour_cost = 60;
int off_peak_hour_cost = 55;

void chart_action(lv_event_t *e) {
  static int32_t last_id = -1;
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  lv_chart_series_t *ser_2 = lv_chart_get_series_next(obj, NULL);
  lv_chart_series_t *ser_1 = lv_chart_get_series_next(obj, ser_2);
  chart_event_data_t *chart_data = (chart_event_data_t *)lv_event_get_user_data(e);

  if (code == LV_EVENT_VALUE_CHANGED) {
    last_id = lv_chart_get_pressed_point(obj);
    if (last_id != LV_CHART_POINT_NONE) {
      lv_chart_set_cursor_point(obj, chart_data->cursor_primary_y, NULL, last_id);
    }
  } else if (code == LV_EVENT_DRAW_PART_END) {
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
    if (!lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_CURSOR)) return;
    if (dsc->p1 == NULL || dsc->p2 == NULL || dsc->p1->y != dsc->p2->y || last_id < 0) return;

    lv_coord_t *data_array_1 = lv_chart_get_y_array(obj, ser_1);
    lv_coord_t *data_array_2 = lv_chart_get_y_array(obj, ser_2);

    char buf[30];

    if (data_array_1[last_id] == CHART_DATA_VALUE_IS_NAN ||
        data_array_2[last_id] == CHART_DATA_VALUE_IS_NAN)
      lv_snprintf(buf, sizeof(buf), "Invalid Value, Invalid Value");
    else
      lv_snprintf(buf, sizeof(buf), "%.*f kWh, %.*f Rs",
                  CHART_DATA_DECIMAL_POINT_PERCISION,
                  data_array_1[last_id] /
                      (float)pow(10, (double)CHART_DATA_DECIMAL_POINT_PERCISION),
                  CHART_DATA_DECIMAL_POINT_PERCISION,
                  data_array_2[last_id] /
                      (float)pow(10,(double)CHART_DATA_DECIMAL_POINT_PERCISION));

    lv_draw_label_dsc_t draw_label_dsc;
    lv_draw_label_dsc_init(&draw_label_dsc);
    draw_label_dsc.color = lv_color_black();
    draw_label_dsc.font = &lv_font_montserrat_18;

    lv_point_t size;
    lv_txt_get_size(&size, buf, draw_label_dsc.font, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

    lv_area_t a;
    a.y2 = dsc->p1->y - 5;
    a.y1 = a.y2 - size.y - 10;
    a.x1 = dsc->p1->x + 10;
    a.x2 = a.x1 + size.x + 10;

    lv_draw_rect_dsc_t draw_rect_dsc;
    lv_draw_rect_dsc_init(&draw_rect_dsc);
    draw_rect_dsc.bg_color = lv_palette_main(LV_PALETTE_BLUE);
    draw_rect_dsc.radius = 3;


    lv_draw_rect(dsc->draw_ctx, &draw_rect_dsc, &a);

    a.x1 += 5;
    a.x2 -= 5;
    a.y1 += 5;
    a.y2 -= 5;

    lv_draw_label(dsc->draw_ctx, &draw_label_dsc, &a, buf, NULL);
  }

  else if (code == LV_EVENT_DRAW_PART_BEGIN) {

    //Get the event descriptor
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);

    //Check what part we are updating. Only proceed if we are updating one of the tick label(s)
    if (!lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_TICK_LABEL))
      return;

    //Check this is a callback for a major tick (minor ticks have 0 here)
    if (dsc->text == NULL)
      return;

    if (dsc->id == LV_CHART_AXIS_PRIMARY_X) {
      switch (chart_data->chart_type) {
        case HOURLY_CHART:
          {
            enum half_of_day {
              FIRST_HALF_OF_DAY = 0,  // from 12 am to 12 pm
              SECOND_HALF_OF_DAY,     // 12pm to 12 am
              HOURS_PER_HALF = 12
            };

            uint8_t current_hour_from_zero = dsc->value;
            uint8_t current_hour = dsc->value + 1;
            if (current_hour_from_zero / HOURS_PER_HALF == FIRST_HALF_OF_DAY) {
              if (current_hour % HOURS_PER_HALF == 0) {
                lv_snprintf(dsc->text, dsc->text_length, "12\nPM");
                break;
              }

              lv_snprintf(dsc->text, dsc->text_length, "%d", dsc->value + 1);
              break;
            }

            if (current_hour_from_zero / HOURS_PER_HALF == SECOND_HALF_OF_DAY) {
              if (current_hour % HOURS_PER_HALF == 0) {
                lv_snprintf(dsc->text, dsc->text_length, "12\nAM");
                break;
              }

              lv_snprintf(dsc->text, dsc->text_length, "%d", current_hour % HOURS_PER_HALF);
              break;
            }
          }

        case DAILY_CHART:
          {
            lv_snprintf(dsc->text, dsc->text_length, "%d", dsc->value + 1);
            break;
          }

        case MONTHLY_CHART:
          {
            lv_snprintf(dsc->text, dsc->text_length, "%s", month_names[dsc->value]);
            break;
          }

        default:
          break;
      }
    }
  }
}
void update_hourly_chart() {
  // update hourly chart
  
}

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
  maximum_unit += ceil(maximum_unit * 0.50);
  maximum_cost += ceil(maximum_cost * 0.50);

  power_monitor_daily_units_array[DAILY_ARRAY_SIZE] = minimum_unit;
  power_monitor_daily_cost_array[DAILY_ARRAY_SIZE] = minimum_cost;
  power_monitor_daily_units_array[DAILY_ARRAY_SIZE + 1] = maximum_unit;
  power_monitor_daily_cost_array[DAILY_ARRAY_SIZE + 1] = maximum_cost;
  for (uint32_t i = 0; i < DAILY_ARRAY_SIZE; i++) {
  Serial.print("Units[");
  Serial.print(i);
  Serial.print("]: ");
  Serial.println(power_monitor_daily_units_array[i]);
  Serial.print("Cost[");
  Serial.print(i);
  Serial.print("]: ");
  Serial.println(power_monitor_daily_cost_array[i]);
}
  
  // ** Update Chart Ranges **
  if (daily_chart != NULL) {
  lv_chart_refresh(daily_chart);  // Clear chart before updating
  for (uint32_t i = 0; i < DAILY_ARRAY_SIZE; i++) {
    lv_chart_set_next_value(daily_chart, units_series, power_monitor_daily_units_array[i]);
    lv_chart_set_next_value(daily_chart, cost_series, power_monitor_daily_cost_array[i]);
}
   lv_chart_set_range(daily_chart, LV_CHART_AXIS_PRIMARY_Y,
                   power_monitor_daily_units_array[DAILY_ARRAY_SIZE],
                   power_monitor_daily_units_array[DAILY_ARRAY_SIZE + 1]);

lv_chart_set_range(daily_chart, LV_CHART_AXIS_SECONDARY_Y,
                   power_monitor_daily_cost_array[DAILY_ARRAY_SIZE],
                   power_monitor_daily_cost_array[DAILY_ARRAY_SIZE + 1]);
    lv_chart_refresh(daily_chart);
  }
}


