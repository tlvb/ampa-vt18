#ifndef _bank_h_
#define _bank_h_

#include "gui.h"

typedef uint_fast8_t bank_index;
#define BANK_BPM           0
#define BANK_ONOFF         1
#define BANK_MASKN         2
#define BANK_INHSV         3
#define BANK_HOLDHSV       4
#define BANK_OUTHSV        5
#define BANK_WHITEONOFF    6
#define BANK_STROBEONOFF   7
#define BANK_STROBEAMOUNT  8
#define BANK_UVAMOUNT      9
#define BANK_ZOOM         10
#define BANK_PATTERN      11
typedef uint_fast8_t rangespec;
#define BANK_HUEAVG    0
#define BANK_HUEWINDOW 1
#define BANK_VALMAX    2
#define BANK_VALWINDOW 3

typedef struct {
  float hueavg;
  float huewindow;
  float valmax;
  float valwindow;
} color_range;

typedef struct {
  size_t bpm;
  size_t period;
  bool on;
  size_t mask;
  color_range in_colors;
  color_range hold_colors;
  color_range out_colors;
  bool white;
  float strobe;
  float strobe_amount;
  float uv_amount;
  size_t zoom;
  size_t pattern;
} property_bank;

void initialize_bank(property_bank *b);
void update_bank_bpm(property_bank *b, gui_widget *gw, int16_t delta);
void update_bank_onoff(property_bank *b, gui_widget *gw);
void update_bank_maskn(property_bank *b, gui_widget *gw, int16_t delta);
void update_bank_hsv(property_bank *b, gui_widget *gw, bank_index id, int16_t delta, rangespec rs);
void update_bank_white(property_bank *b, gui_widget *gw);
void update_bank_strobe(property_bank *b, gui_widget *gw);
void update_bank_strobeamount(property_bank *b, gui_widget *gw, int16_t delta);
void update_bank_uvamount(property_bank *b, gui_widget *gw, int16_t delta);
void update_bank_zoom(property_bank *b, gui_widget *gw, int16_t delta);
void update_bank_pattern(property_bank *b, gui_widget *gw, int16_t delta);

void clone_bank_bpm(property_bank *tb, gui_widget *tgw, property_bank *sb, gui_widget *sgw);
void clone_bank_mask(property_bank *tb, gui_widget *tgw, property_bank *sb, gui_widget *sgw);
void clone_bank_hsv(property_bank *tb, gui_widget *tgw, bank_index tid, property_bank *sb, gui_widget *sgw, bank_index sid);
void clone_bank_strobeamount(property_bank *tb, gui_widget *tgw, property_bank *sb, gui_widget *sgw);
void clone_bank_uvamount(property_bank *tb, gui_widget *tgw, property_bank *sb, gui_widget *sgw);
void clone_zoom(property_bank *tb, gui_widget *tgw, property_bank *sb, gui_widget *sgw);
void clone_pattern(property_bank *tb, gui_widget *tgw, property_bank *sb, gui_widget *sgw);
#endif
