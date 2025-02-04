#ifndef SIDE_MENU_H
#define SIDE_MENU_H

#include <lvgl.h>
//static lv_obj_t *menu;

// Declare the function for setting up the side menu
void side_menu(void);

// Declare the function for handling the back button event
void back_event_handler(lv_event_t * e);

//void menu_button_event_handler(lv_event_t *e) 


#endif // SIDE_MENU_H
