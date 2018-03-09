#ifndef _light_h_
#define _light_h_

#include <stdbool.h>
#include <inttypes.h>
#include "util.h"
#include "gui.h"
#include "bank.h"

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

#define SOFTCHANNELS_N 12

#define HUE         0
#define SATURATION  1
#define VALUE       2
#define RED         3
#define GREEN       4
#define BLUE        5
#define WHITE       6
#define STROBE      7
#define ULTRAVIOLET 8
#define PAN         9
#define TILT       10
#define ZOOM       11

#define HARDCHANNELS_N 32

#define TRI_PER_SINGLECHANNEL 2
#define TRI_PER_LEDPAR56 TRI_PER_SINGLECHANNEL
#define TRI_PER_LEDWASH 6
#define TRI_PER_LEDMH 8
#define TRI_PER_LEDMH2 11

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

typedef struct {
  size_t n;
  mask_state *mask;
  fixture *fix;
  property_bank *bank;
} program_data;


size_t fixture_widget_triangle_count(const fixture_widget *fw);
void   assign_fixture_widget_model_data(model *global, fixture_widget *fw);
void   initialize_fixture_widget(model *global, fixture_widget *fw, fixture *fix, screen_dim x, screen_dim y, screen_dim w, screen_dim h);
void   update_fixture_widgets(fixture_widget *fws, size_t n);

void interpolate_values(fixture *fix);


#endif
