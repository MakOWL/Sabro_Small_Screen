//#include "screens.h"
#include "side_menu.h"
#include "widget_dec.h"
#include <Arduino.h>

// Global screen objects
lv_obj_t *main_screen;
lv_obj_t *data_screen;
lv_obj_t *timer_screen;
lv_obj_t *temp_label;
lv_obj_t *temp_dial;

// Mode options
const char *modes[] = {"Auto", "Cool", "Heat", "Fan", "Dry"};
int mode_index = 0;

static bool mode_label_clickable = true; // Flag to track if clickable
static lv_timer_t *reset_timer = NULL;   // Timer to reset the flag

void reset_clickable_timer(lv_timer_t *timer) {
    mode_label_clickable = true;
    lv_timer_del(timer); // Delete the timer once done
    reset_timer = NULL;
}

// Event Handlers
void menu_button_event_handler(lv_event_t *e) {
    side_menu(); // Handle menu visibility
}

void mode_label_event_handler(lv_event_t *e) {
     if (!mode_label_clickable) {
        return; // Ignore input if still in delay period
    }
    mode_label_clickable = false;

    // Start a timer for a delay (e.g., 1000 ms)
    reset_timer = lv_timer_create(reset_clickable_timer, 1000, NULL);
    lv_obj_t *label = lv_event_get_target(e);
    mode_index = (mode_index + 1) % (sizeof(modes) / sizeof(modes[0]));
    lv_label_set_text(label, modes[mode_index]);
    Serial.print(modes[mode_index]);
}

void temp_handler(lv_event_t *e) {
    lv_obj_t *arc = lv_event_get_target(e);
    lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e);
    int32_t value = lv_arc_get_value(arc);
    char value_str[8];
    sprintf(value_str, "%d", value);
    lv_label_set_text(label, value_str);
}

void button_event_handler(lv_event_t *e) {
    lv_obj_t *button = lv_event_get_target(e);
    const char *button_text = lv_label_get_text(lv_obj_get_child(button, 0));
    int32_t value = lv_arc_get_value(temp_dial);

    if (strcmp(button_text, "+") == 0) value++;
    else if (strcmp(button_text, "-") == 0) value--;

    if (value < 0) value = 0;
    if (value > 30) value = 30;

    lv_arc_set_value(temp_dial, value);

    char value_str[8];
    sprintf(value_str, "%d", value);
    lv_label_set_text(temp_label, value_str);
}

// Main Screen
void create_main_screen() {
    if (main_screen != NULL) {
        lv_scr_load(main_screen); // Load the existing screen
        return;
    }
    main_screen = lv_obj_create(NULL);
    lv_obj_set_pos(main_screen, 0, 0);
    lv_obj_set_size(main_screen, 240, 320);

    lv_obj_t *date_time_label = lv_label_create(main_screen);
    lv_obj_set_pos(date_time_label, 85, 10);
    lv_obj_set_size(date_time_label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_label_set_text(date_time_label, "10 Oct 2024, 6:30 pm");
    lv_obj_set_style_text_font(date_time_label, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_align(date_time_label, LV_ALIGN_OUT_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Menu button
    lv_obj_t *menu_btn = lv_btn_create(main_screen);
    lv_obj_set_pos(menu_btn, 4, 6);
    lv_obj_set_size(menu_btn, 33, 24);

    lv_obj_t *menu_label = lv_label_create(menu_btn);
    lv_label_set_text(menu_label, "");
    lv_obj_set_style_align(menu_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(menu_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(menu_btn, menu_button_event_handler, LV_EVENT_CLICKED, NULL);

    // Temperature dial
    temp_dial = lv_arc_create(main_screen);
    lv_obj_set_pos(temp_dial, 20, 80);
    lv_obj_set_size(temp_dial, 204, 204);
    lv_arc_set_range(temp_dial, 16, 30);
    lv_arc_set_value(temp_dial, 25);
    lv_arc_set_bg_end_angle(temp_dial, 45);
    lv_arc_set_mode(temp_dial, LV_ARC_MODE_NORMAL);

    temp_label = lv_label_create(main_screen);
    lv_obj_set_pos(temp_label, 97, 150);
    lv_label_set_text(temp_label, "25");
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_46, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(temp_dial, temp_handler, LV_EVENT_VALUE_CHANGED, temp_label);

    // "+" Button
    lv_obj_t *inc_button = lv_btn_create(main_screen);
    lv_obj_set_pos(inc_button, 128, 256);
    lv_obj_set_size(inc_button, 50, 25);
    lv_obj_add_event_cb(inc_button, button_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t *inc_label = lv_label_create(inc_button);
    lv_label_set_text(inc_label, "+");
    lv_obj_set_style_align(inc_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(inc_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // "-" Button
    lv_obj_t *dec_button = lv_btn_create(main_screen);
    lv_obj_set_pos(dec_button, 62, 256);
    lv_obj_set_size(dec_button, 50, 25);
    lv_obj_add_event_cb(dec_button, button_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t *dec_label = lv_label_create(dec_button);
    lv_label_set_text(dec_label, "-");
    lv_obj_set_style_align(dec_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(dec_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Mode label
    lv_obj_t *mode_label = lv_label_create(main_screen);
    lv_obj_set_pos(mode_label, 95, 200);
    lv_label_set_text(mode_label, modes[1]);
    lv_obj_set_style_text_font(mode_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(mode_label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(mode_label, mode_label_event_handler, LV_EVENT_CLICKED, NULL);

    lv_scr_load(main_screen);
}

// Data Screen
void create_data_screen() {
    if (data_screen != NULL) {
        lv_scr_load(data_screen); // Load the existing screen
        return;
    }
    data_screen = lv_obj_create(NULL);
    lv_obj_set_pos(data_screen, 0, 0);
    lv_obj_set_size(data_screen, 240, 320);
        // Menu button
    lv_obj_t *menu_btn = lv_btn_create(data_screen);
    lv_obj_set_pos(menu_btn, 4, 6);
    lv_obj_set_size(menu_btn, 33, 24);

    lv_obj_t *menu_label = lv_label_create(menu_btn);
    lv_label_set_text(menu_label, "");
    lv_obj_set_style_align(menu_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(menu_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(menu_btn, menu_button_event_handler, LV_EVENT_CLICKED, NULL);
    // Additional elements for the data screen can be added here.
    lv_obj_t *parent_obj = data_screen;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 80, 117);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Data Screen");
        }
        
}

void create_timer_screen() {
    if (timer_screen != NULL) {
        lv_scr_load(timer_screen); // Load the existing screen
        return;
    }
    timer_screen = lv_obj_create(NULL);
    lv_obj_set_pos(timer_screen, 0, 0);
    lv_obj_set_size(timer_screen, 240, 320);
        // Menu button
    lv_obj_t *menu_btn = lv_btn_create(timer_screen);
    lv_obj_set_pos(menu_btn, 4, 6);
    lv_obj_set_size(menu_btn, 33, 24);

    lv_obj_t *menu_label = lv_label_create(menu_btn);
    lv_label_set_text(menu_label, "");
    lv_obj_set_style_align(menu_label, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(menu_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(menu_btn, menu_button_event_handler, LV_EVENT_CLICKED, NULL);
    // Additional elements for the data screen can be added here.

    lv_obj_t *parent_obj = timer_screen;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 80, 117);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Timer Screen");
        }
        
}
