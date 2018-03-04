#ifndef _light_h_
#define _light_h_

#include <stdbool.h>
#include <inttypes.h>
#include "util.h"
#include "gui.h"

typedef uint_fast8_t light_type;   // if we would ever grow to support more than 256 channels this should change
typedef float soft_value;          // float should offer enough precision for any applications
typedef uint_fast8_t mask_state;
typedef uint_fast16_t dmx_channel; // possible dmx protocol channel values are 1 to 512 inclusive
typedef uint_fast8_t dmx_value;    // possible dmx protocol "value" values are 0 to 255 inclusive


#define SINGLECHANNEL 0
#define LEDPAR56      1
#define LEDWASH       2
#define LEDMH         3
#define LEDMH2        4

#define MASK_ZERO    0
#define MASK_IN      1
#define MASK_SUSTAIN 2
#define MASK_OUT     3

#define INTERPOLATE_N 3
#define START   0
#define END     1
#define CURRENT 2

#define SOFTCHANNELS_N 9

#define HUE         0
#define SATURATION  1
#define VALUE       2
#define WHITE       3
#define STROBE      4
#define ULTRAVIOLET 5
#define PAN         6
#define TILT        7
#define ZOOM        8

#define HARDCHANNELS_N 32

typedef struct {
  light_type  type;
  dmx_channel base_channel;
  soft_value  values[INTERPOLATE_N][SOFTCHANNELS_N];
  dmx_value   hw_values[HARDCHANNELS_N];
} fixture;


typedef struct {
  fixture *fix;
  hitbox   dims;
  model    m;
} fixture_widget;


size_t fixture_widget_triangle_count(const fixture_widget *fw);
void   assign_fixture_widget_model_data(model *global, fixture_widget *fw);
void   initialize_fixture_widget(fixture_widget *fw, screen_dim x, screen_dim y, screen_dim w, screen_dim h);

#endif
