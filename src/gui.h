#ifndef _gui_h_
#define _gui_h_


#define INACTIVE   0
#define BUTTON     1
#define SWATCH2    2
#define SWATCH4    3
#define TWODIGITS  4
#define FOURDIGITS 5

#define TRI_PER_INACTIVE  2
#define TRI_PER_BUTTON    4
#define TRI_PER_SWATCH2   4
#define TRI_PER_SWATCH4   6
#define TRI_PER_7SEG      (7*2)
#define TRI_PER_TWODIGIT  (4+2*TRI_PER_7SEG)
#define TRI_PER_FOURDIGIT (4+4*TRI_PER_7SEG)

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
  float red[4];
  float green[4];
  float blue[4];
} swatch_data;
typedef struct {
  screen_dim pos[2];
	screen_dim s;
	GLfloat *points;
	GLfloat *colors;
	GLfloat off_color[3];
	GLfloat on_color[3];
} digit;
typedef struct {
  screen_dim pos[2];
  screen_dim size[2];
	GLfloat *points;
	GLfloat *colors;
	int32_t number;
	digit digits[4];
} digit_data;

typedef struct {
  gui_widget_type type;
  bool updated;
  bool select; // programatically targeted e.g. by a 'select all' operation
  bool hover;  // targeted by specifically hovering the mouse above the widget
  union {
    button_data button;
    swatch_data swatch;
    digit_data  digits;
  };
  hitbox dims;
  model  m;
} gui_widget;

typedef struct {
  float hueavg;
  float huewindow;
  float valmax;
  float valwindow;
} color_range;

typedef struct {
  bool on;
  size_t mask;
  color_range in_colors;
  color_range hold_colors;
  color_range out_colors;
  float strobe_amount;
  float uv_amount;
  size_t pattern;
} bank;

bool is_inside(const hitbox *h, screen_dim x, screen_dim y);
void initialize_gui_widget(model *global, gui_widget *gw, screen_dim x, screen_dim y, screen_dim w, screen_dim h, gui_widget_type type);
void update_gui_widgets_graphics(gui_widget *gw, size_t n, bool heart);

#endif
