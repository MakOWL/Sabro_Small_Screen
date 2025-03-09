#include "side_menu.h"
#include "widget_dec.h"

static lv_obj_t *menu;
lv_obj_t *main_page;
lv_obj_t *power_monitor_page;

void back_event_handler(lv_event_t *e) {
  lv_obj_t *obj = lv_event_get_target(e);
  lv_obj_t *menu_data = (lv_obj_t *)lv_event_get_user_data(e);

  if (lv_menu_back_btn_is_root(menu_data, obj)) {
    lv_obj_add_flag(menu_data, LV_OBJ_FLAG_HIDDEN);
  }
}

// Menu item event handler
void menu_item_event_handler(lv_event_t *e){
    lv_obj_t *obj = lv_event_get_target(e);
    const char *item_text = lv_label_get_text(lv_obj_get_child(obj, 0));
    
    // Navigate to the appropriate screen based on the clicked item
    if (strcmp(item_text, "Main Screen") == 0) {
        create_main_screen();
        lv_scr_load(main_screen); 
        lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
    } else if (strcmp(item_text, "Data Screen") == 0) {
        create_data_screen();
        lv_scr_load(data_screen); 
        lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
    }  else if (strcmp(item_text, "Timer Screen") == 0) {
        create_timer_screen();
        lv_scr_load(timer_screen); 
        lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
    }  else if (strcmp(item_text, "Additional Modes Screen") == 0) {
        create_additional_modes_screen();
        lv_scr_load(add_mod_screen); 
        lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
    } else if (strcmp(item_text, "Settings") == 0) {
        create_setting_screen();
        lv_scr_load(setting_screen); 
        lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
    } else if (strcmp(item_text, "Power Monitor") == 0) {
        power_monitor_menu();
        lv_menu_set_page(menu, power_monitor_page);
        lv_obj_clear_flag(menu, LV_OBJ_FLAG_HIDDEN); 
    } 
}
void menu_power_monitor_event_handler(lv_event_t *e){
    lv_obj_t *obj = lv_event_get_target(e);
    const char *item_text = lv_label_get_text(lv_obj_get_child(obj, 0));
    if (strcmp(item_text, "Daily") == 0) {
        create_screen_power_monitor_daily_screen();
        lv_scr_load(power_monitor_daily_screen);
        lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
    } else if(strcmp(item_text,"Hourly") == 0){
        create_screen_power_monitor_hourly_screen();
        lv_scr_load(power_monitor_hourly_screen);
        lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
    } else if (strcmp(item_text, "Monthly") == 0) {    
        lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
    }
}

void power_monitor_menu(void){
  power_monitor_page = lv_menu_page_create(menu,"Power Monitor");

  lv_obj_t *cont = lv_menu_cont_create(power_monitor_page);
  lv_obj_t *label = lv_label_create(cont);
  lv_label_set_text(label, "Hourly");
  lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(cont,  menu_power_monitor_event_handler, LV_EVENT_CLICKED, NULL);

  cont = lv_menu_cont_create(power_monitor_page);
  label = lv_label_create(cont);
  lv_label_set_text(label, "Daily");
  lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(cont,  menu_power_monitor_event_handler, LV_EVENT_CLICKED, NULL);

  cont = lv_menu_cont_create(power_monitor_page);
  label = lv_label_create(cont);
  lv_label_set_text(label, "Monthly");
  lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(cont,  menu_power_monitor_event_handler, LV_EVENT_CLICKED, NULL);

  lv_menu_set_page(menu, power_monitor_page);
}

// Side menu function
void side_menu(void) {
    menu = NULL;

    if (!menu) {
        // Create menu
         menu = lv_menu_create(lv_scr_act());
        lv_menu_set_mode_root_back_btn(menu, LV_MENU_ROOT_BACK_BTN_ENABLED);
        lv_obj_t *back_btn = lv_menu_get_main_header_back_btn(menu);
        lv_obj_t *back_btn_label = lv_label_create(back_btn);
        lv_label_set_text(back_btn_label, "Back");
        lv_obj_add_event_cb(back_btn, back_event_handler, LV_EVENT_CLICKED, menu);
        lv_obj_set_size(menu, 200, 300);
        lv_obj_set_pos(menu, 0, 0);

        main_page = lv_menu_page_create(menu, NULL);

        // Add "Main Screen" option
        lv_obj_t *cont = lv_menu_cont_create(main_page);
        lv_obj_t *label = lv_label_create(cont);
        lv_label_set_text(label, "Main Screen");
        lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(cont, menu_item_event_handler, LV_EVENT_CLICKED, NULL);

        
        cont = lv_menu_cont_create(main_page);
        label = lv_label_create(cont);
        lv_label_set_text(label, "Additional Modes Screen");
        lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(cont, menu_item_event_handler, LV_EVENT_CLICKED, NULL);

        // Add "Data Screen" option
        cont = lv_menu_cont_create(main_page);
        label = lv_label_create(cont);
        lv_label_set_text(label, "Data Screen");
        lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(cont, menu_item_event_handler, LV_EVENT_CLICKED, NULL);

        // Add "Data Screen" option
        cont = lv_menu_cont_create(main_page);
        label = lv_label_create(cont);
        lv_label_set_text(label, "Timer Screen");
        lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(cont, menu_item_event_handler, LV_EVENT_CLICKED, NULL);

        // Add "Power Monitor" option
        cont = lv_menu_cont_create(main_page);
        label = lv_label_create(cont);
        lv_label_set_text(label, "Power Monitor");
        lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(cont, menu_item_event_handler, LV_EVENT_CLICKED, NULL);


        cont = lv_menu_cont_create(main_page);
        label = lv_label_create(cont);
        lv_label_set_text(label, "Settings");
        lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(cont, menu_item_event_handler, LV_EVENT_CLICKED, NULL);

        // Set the menu page
        lv_menu_set_page(menu, main_page);
    }

    
}