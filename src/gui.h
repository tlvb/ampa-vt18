#ifndef _gui_h_
#define _gui_h_

#define TRI_PER_BUTTON 4
#define TRI_PER_SWATCH 4

#define BUTTON 0
#define SWATCH 1

#include "util.h"
#include <stdbool.h>
#include <stdint.h>

typedef uint_fast16_t screen_dim; // dimension large enough to hold any component of a onscreen pixel position
#define PRI_SCD PRIuFAST16

typedef uint_fast8_t gui_widget_type;
#define PRI_GWT PRIuFAST8

typedef struct {
  screen_dim pos[2];
  screen_dim size[2];
} hitbox;

typedef struct {
  bool pressed; // current activation
  bool on;      // toggle state
} button_data;

typedef struct {
  gui_widget_type type;
  bool select; // programatically targeted e.g. by a 'select all' operation
  bool hover;  // targeted by specifically hovering the mouse above the widget
  union {
    button_data button;
  };
  hitbox dims;
  model  m;
} gui_widget;

bool is_inside(const hitbox *h, screen_dim x, screen_dim y);
void initialize_gui_widget(model *global, gui_widget *gw, screen_dim x, screen_dim y, screen_dim w, screen_dim h, gui_widget_type type);
void update_gui_widget_graphics(gui_widget *gw, bool heart);

#endif
