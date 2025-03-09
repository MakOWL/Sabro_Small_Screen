#ifndef IMAGE_COLOURS_H
#define IMAGE_COLOURS_H
#define FIXED_COLOR(color)  (((color & 0x0000FF) << 16) | (color & 0x00FF00) | ((color & 0xFF0000) >> 16))

extern lv_color_t auto_color;
extern lv_color_t cool_color;  
extern lv_color_t heat_color;  
extern lv_color_t fan_color;
extern lv_color_t dry_color;
extern lv_color_t default_color;

   enum color_codes
  {
      COLOR_INCUBUS = 0x722626,
      COLOR_SILKEN_RUBY = 0xe61717,
      COLOR_LIMA = 0x3ce221,
      COLOR_BILBAO = 0x167016,
      COLOR_RED = 0xeb4030
  };
#endif