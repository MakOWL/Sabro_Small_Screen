#include "side_menu.h"
#include "widget_dec.h"

static lv_obj_t *menu;

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
        lv_scr_load(main_screen); // Navigate to Main Screen
    } else if (strcmp(item_text, "Data Screen") == 0) {
        create_data_screen();
        lv_scr_load(data_screen); // Navigate to Data Screen
    }  else if (strcmp(item_text, "Timer Screen") == 0) {
        create_timer_screen();
        lv_scr_load(timer_screen); // Navigate to timer Screen
    }  else if (strcmp(item_text, "Additional Modes Screen") == 0) {
        create_additional_modes_screen();
        lv_scr_load(add_mod_screen); // Navigate to timer Screen
    }
    
    // Hide the menu after navigation
    lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
}

// Side menu function
void side_menu(void) {
    menu = NULL;

    if (!menu) {
        // Create menu
        menu = lv_menu_create(lv_scr_act());
        lv_menu_set_mode_root_back_btn(menu, LV_MENU_ROOT_BACK_BTN_ENABLED);
        lv_obj_add_event_cb(menu, back_event_handler, LV_EVENT_CLICKED, menu);
        lv_obj_set_size(menu, 200, 300);
        lv_obj_set_pos(menu, 0, 0);

        // Create main page
        lv_obj_t *main_page = lv_menu_page_create(menu, NULL);

        // Add "Main Screen" option
        lv_obj_t *cont = lv_menu_cont_create(main_page);
        lv_obj_t *label = lv_label_create(cont);
        lv_label_set_text(label, "Main Screen");
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

        cont = lv_menu_cont_create(main_page);
        label = lv_label_create(cont);
        lv_label_set_text(label, "Additional Modes Screen");
         lv_obj_add_flag(cont, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(cont, menu_item_event_handler, LV_EVENT_CLICKED, NULL);

        // Set the menu page
        lv_menu_set_page(menu, main_page);
    }

    // Show the menu
    lv_obj_clear_flag(menu, LV_OBJ_FLAG_HIDDEN);
}
