#ifndef CHARTS_H
#define CHARTS_H
#include <lvgl.h>
#include <cmath>
#include <esp_now.h>

enum precison_enum{
  CHART_DATA_DECIMAL_POINT_PERCISION = 2,
  CHART_DATA_VALUE_IS_NAN = -1
};

enum chart_sizes { HOURLY_ARRAY_SIZE = 24,
                   DAILY_ARRAY_SIZE = 31,
                   MONTHLY_ARRAY_SIZE = 12
};

enum chart_types { HOURLY_CHART = 0,
                   DAILY_CHART,
                   MONTHLY_CHART
};

typedef struct
{
  lv_chart_cursor_t *cursor_primary_y;
  lv_chart_cursor_t *cursor_secondary_y;
  uint8_t chart_type;
} chart_event_data_t;

extern lv_chart_series_t *units_series;
extern lv_chart_series_t *cost_series;
extern lv_obj_t *power_monitor_hourly_data_chart;
extern lv_obj_t *power_monitor_daily_data_chart;
extern lv_obj_t *power_monitor_monthly_data_chart;

extern lv_coord_t power_monitor_hourly_units_array[HOURLY_ARRAY_SIZE + 2];
extern lv_coord_t power_monitor_daily_units_array[DAILY_ARRAY_SIZE + 2];
extern lv_coord_t power_monitor_monthly_cost_array[MONTHLY_ARRAY_SIZE + 2];

extern lv_coord_t power_monitor_hourly_cost_array[HOURLY_ARRAY_SIZE + 2];
extern lv_coord_t power_monitor_daily_cost_array[DAILY_ARRAY_SIZE + 2];
extern lv_coord_t power_monitor_monthly_units_array[MONTHLY_ARRAY_SIZE + 2];

extern float esp_now_hourly_units_array[HOURLY_ARRAY_SIZE];
extern float esp_now_daily_off_peak_units_array[DAILY_ARRAY_SIZE];
extern float esp_now_daily_peak_units_array[DAILY_ARRAY_SIZE];
extern float esp_now_monthly_off_peak_units_array[MONTHLY_ARRAY_SIZE];
extern float esp_now_monthly_peak_units_array[MONTHLY_ARRAY_SIZE];


void chart_action(lv_event_t *e);
void update_hourly_chart();
void update_daily_chart();
void update_monthly_chart();
void cursor_action(); // this will be common in all the files 


#endif