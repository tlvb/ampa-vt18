#ifndef _gui_h_
#define _gui_h_


#define INACTIVE 0
#define BUTTON   1
#define SWATCH2  2
#define SWATCH4  3

#define TRI_PER_INACTIVE 2
#define TRI_PER_BUTTON   4
#define TRI_PER_SWATCH2  4
#define TRI_PER_SWATCH4  6

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
  float red[2];
  float green[2];
  float blue[2];
} swatch_data;

typedef struct {
  gui_widget_type type;
  bool updated;
  bool select; // programatically targeted e.g. by a 'select all' operation
  bool hover;  // targeted by specifically hovering the mouse above the widget
  union {
    button_data button;
    swatch_data swatch;
  };
  hitbox dims;
  model  m;
} gui_widget;

bool is_inside(const hitbox *h, screen_dim x, screen_dim y);
void initialize_gui_widget(model *global, gui_widget *gw, screen_dim x, screen_dim y, screen_dim w, screen_dim h, gui_widget_type type);
void update_gui_widgets_graphics(gui_widget *gw, size_t n, bool heart);

#endif
