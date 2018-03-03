#ifndef _gui_h_
#define _gui_h_

#include <inttypes.h>

typedef uint_fast16_t screen_dim; // dimension large enough to hold any component of a onscreen pixel position

typedef uint_fast8_t gui_widget_type;

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






bool is_inside(const *hitbox h, screen_dim x, screen_dim y);

#endif
