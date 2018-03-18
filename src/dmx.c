#include "dmx.h"

/* void map_hwchannels(fixture *fix) */
/* { */
/*   if (fix->type == LEDWASH) { */
/*     // 8 channel mode */
/*     // R G B W 0 T 0 255 */
/*     conduit_send(con, fix->base_channel,   floorf(fix->values[CURRENT][RED]*255.0f)); */
/*     conduit_send(con, fix->base_channel+1, floorf(fix->values[CURRENT][GREEN]*255.0f)); */
/*     conduit_send(con, fix->base_channel+2, floorf(fix->values[CURRENT][BLUE]*255.0f)); */
/*     conduit_send(con, fix->base_channel+3, floorf(fix->values[CURRENT][WHITE]*255.0f)); */
/*     conduit_send(con, fix->base_channel+5, floorf(fix->values[CURRENT][strobe]*255.0f)); */
/*   } */
/*   else if (fix->type == LEDMH) { */
/*     // 14 channel mode */
/*     // 1   2    3    4     6 7   8 9 10 11 12 13 14 */
/*     // pan panf tilt tiltf 0 255 r g  b  w  0  t  0 */

/*     uint16_t pan16 = fix->pan_m+floorf(lights->mh[i].pan->value[CURRENT]*fix->pan_k); */
/*     uint16_t tilt16 = fix->tilt_m+floorf(lights->mh[i].tilt->value[CURRENT]*fix->tilt_k); */

/*     conduit_send(con, fix->base_channel,    pan16>>8); */
/*     conduit_send(con, fix->base_channel+ 1, pan16&255); */
/*     conduit_send(con, fix->base_channel+ 2, tilt16>>8); */
/*     conduit_send(con, fix->base_channel+ 3, tilt16&255); */
/*     conduit_send(con, fix->base_channel+ 6, floorf(fix->values[CURRENT][RED]*255.0f)); */
/*     conduit_send(con, fix->base_channel+ 7, floorf(fix->values[CURRENT][GREEN]*255.0f)); */
/*     conduit_send(con, fix->base_channel+ 8, floorf(fix->values[CURRENT][BLUE]*255.0f)); */
/*     conduit_send(con, fix->base_channel+ 9, floorf(fix->values[CURRENT][WHITE]*255.0f)); */
/*     conduit_send(con, fix->base_channel+11, floorf(fix->values[CURRENT][STROBE]*255.0f)); */
/*   } */
/*   else if (fix->type == LEDMH2) { */
/*     // 21 channel mode */
/*     // 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 2 */
/*     // pc pf tc tf  r  g  b  w  0  u  0  t  + */

/*     uint16_t pan16 = fix->pan_m+floorf(lights->mh[i].pan->value[CURRENT]*fix->pan_k); */
/*     uint16_t tilt16 = fix->tilt_m+floorf(lights->mh[i].tilt->value[CURRENT]*fix->tilt_k); */

/*     conduit_send(con, fix->base_channel,    pan16>>8); */
/*     conduit_send(con, fix->base_channel+ 1, pan16&255); */
/*     conduit_send(con, fix->base_channel+ 2, tilt16>>8); */
/*     conduit_send(con, fix->base_channel+ 3, tilt16&255); */
/*     conduit_send(con, fix->base_channel+ 4, floorf(fix->values[CURRENT][RED]*255.0f)); */
/*     conduit_send(con, fix->base_channel+ 5, floorf(fix->values[CURRENT][GREEN]*255.0f)); */
/*     conduit_send(con, fix->base_channel+ 6, floorf(fix->values[CURRENT][BLUE]*255.0f)); */
/*     conduit_send(con, fix->base_channel+ 7, floorf(fix->values[CURRENT][WHITE]*255.0f)); */
/*     conduit_send(con, fix->base_channel+ 9, floorf(fix->values[CURRENT][ULTRAVIOLET]*255.0f)); */
/*     conduit_send(con, fix->base_channel+11, floorf(fix->values[CURRENT][STROBE]*255.0f)); */
/*   } */
/* } */
void write_dmx_statics(conduit *con, const fixture *fix)
{
  if (fix->type == LEDWASH) {
    // 8 channel mode
    // R G B W 0 T 0 255
    conduit_send(con, fix->base_channel,   floorf(fix->values[CURRENT][RED]*255.0f));
    conduit_send(con, fix->base_channel+1, floorf(fix->values[CURRENT][GREEN]*255.0f));
    conduit_send(con, fix->base_channel+2, floorf(fix->values[CURRENT][BLUE]*255.0f));
    conduit_send(con, fix->base_channel+3, floorf(fix->values[CURRENT][WHITE]*255.0f));
    conduit_send(con, fix->base_channel+4, 0);
    conduit_send(con, fix->base_channel+5, floorf(fix->values[CURRENT][STROBE]*255.0f));
    conduit_send(con, fix->base_channel+6, 0);
    conduit_send(con, fix->base_channel+7, 255);
  }
  else if (fix->type == LEDMH) {
    // 14 channel mode
    // 1   2    3    4     6 7   8 9 10 11 12 13 14
    // pan panf tilt tiltf 0 255 r g  b  w  0  t  0

    uint16_t pan16 = fix->pan_m+floorf(fix->values[CURRENT][PAN]*fix->pan_k);
    uint16_t tilt16 = fix->tilt_m+floorf(fix->values[CURRENT][TILT]*fix->tilt_k);

    conduit_send(con, fix->base_channel,    pan16>>8);
    conduit_send(con, fix->base_channel+ 1, pan16&255);
    conduit_send(con, fix->base_channel+ 2, tilt16>>8);
    conduit_send(con, fix->base_channel+ 3, tilt16&255);
    conduit_send(con, fix->base_channel+ 4, 255);
    conduit_send(con, fix->base_channel+ 5, 255);
    conduit_send(con, fix->base_channel+ 6, floorf(fix->values[CURRENT][RED]*255.0f));
    conduit_send(con, fix->base_channel+ 7, floorf(fix->values[CURRENT][GREEN]*255.0f));
    conduit_send(con, fix->base_channel+ 8, floorf(fix->values[CURRENT][BLUE]*255.0f));
    conduit_send(con, fix->base_channel+ 9, floorf(fix->values[CURRENT][WHITE]*255.0f));
    conduit_send(con, fix->base_channel+10, 0);
    conduit_send(con, fix->base_channel+11, floorf(fix->values[CURRENT][STROBE]*255.0f));
    conduit_send(con, fix->base_channel+12, 0);
    conduit_send(con, fix->base_channel+13, 255);
  }
  else if (fix->type == LEDMH2) {
    // 21 channel mode
    // 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 2
    // pc pf tc tf  r  g  b  w  0  u  0  t  +

    uint16_t pan16 = fix->pan_m+floorf(fix->values[CURRENT][PAN]*fix->pan_k);
    uint16_t tilt16 = fix->tilt_m+floorf(fix->values[CURRENT][TILT]*fix->tilt_k);
    uint8_t strobeval = 32+floorf(fix->values[CURRENT][STROBE]*(255.0f-32.0f));


    conduit_send(con, fix->base_channel,    pan16>>8);
    conduit_send(con, fix->base_channel+ 1, pan16&255);
    conduit_send(con, fix->base_channel+ 2, tilt16>>8);
    conduit_send(con, fix->base_channel+ 3, tilt16&255);
    conduit_send(con, fix->base_channel+ 4, floorf(fix->values[CURRENT][RED]*255.0f));
    conduit_send(con, fix->base_channel+ 5, floorf(fix->values[CURRENT][GREEN]*255.0f));
    conduit_send(con, fix->base_channel+ 6, floorf(fix->values[CURRENT][BLUE]*255.0f));
    conduit_send(con, fix->base_channel+ 7, floorf(fix->values[CURRENT][WHITE]*255.0f));
    conduit_send(con, fix->base_channel+ 8, 0);
    conduit_send(con, fix->base_channel+ 9, floorf(fix->values[CURRENT][ULTRAVIOLET]*255.0f));
    conduit_send(con, fix->base_channel+10, 0);
    conduit_send(con, fix->base_channel+11, strobeval);
    conduit_send(con, fix->base_channel+12, 255);
    conduit_send(con, fix->base_channel+13, 0);
    conduit_send(con, fix->base_channel+14, 0);
    conduit_send(con, fix->base_channel+15, 0);
    conduit_send(con, fix->base_channel+16, 0);
    conduit_send(con, fix->base_channel+17, 0);
    conduit_send(con, fix->base_channel+18, 0);
    conduit_send(con, fix->base_channel+19, 0);
    conduit_send(con, fix->base_channel+20, 0);
  }
}
void write_dmx_dynamics(conduit *con, const fixture *fix)
{
  if (fix->type == LEDWASH) {
    // 8 channel mode
    // R G B W 0 T 0 255
    conduit_send(con, fix->base_channel,   floorf(fix->values[CURRENT][RED]*255.0f));
    conduit_send(con, fix->base_channel+1, floorf(fix->values[CURRENT][GREEN]*255.0f));
    conduit_send(con, fix->base_channel+2, floorf(fix->values[CURRENT][BLUE]*255.0f));
    conduit_send(con, fix->base_channel+3, floorf(fix->values[CURRENT][WHITE]*255.0f));
    conduit_send(con, fix->base_channel+5, floorf(fix->values[CURRENT][STROBE]*255.0f));
  }
  else if (fix->type == LEDMH) {
    // 14 channel mode
    // 1   2    3    4     6 7   8 9 10 11 12 13 14
    // pan panf tilt tiltf 0 255 r g  b  w  0  t  0

    uint16_t pan16 = fix->pan_m+floorf(fix->values[CURRENT][PAN]*fix->pan_k);
    uint16_t tilt16 = fix->tilt_m+floorf(fix->values[CURRENT][TILT]*fix->tilt_k);

    conduit_send(con, fix->base_channel,    pan16>>8);
    conduit_send(con, fix->base_channel+ 1, pan16&255);
    conduit_send(con, fix->base_channel+ 2, tilt16>>8);
    conduit_send(con, fix->base_channel+ 3, tilt16&255);
    conduit_send(con, fix->base_channel+ 6, floorf(fix->values[CURRENT][RED]*255.0f));
    conduit_send(con, fix->base_channel+ 7, floorf(fix->values[CURRENT][GREEN]*255.0f));
    conduit_send(con, fix->base_channel+ 8, floorf(fix->values[CURRENT][BLUE]*255.0f));
    conduit_send(con, fix->base_channel+ 9, floorf(fix->values[CURRENT][WHITE]*255.0f));
    conduit_send(con, fix->base_channel+11, floorf(fix->values[CURRENT][STROBE]*255.0f));
  }
  else if (fix->type == LEDMH2) {
    // 21 channel mode
    // 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 2
    // pc pf tc tf  r  g  b  w  0  u  0  t  +

    uint16_t pan16 = fix->pan_m+floorf(fix->values[CURRENT][PAN]*fix->pan_k);
    uint16_t tilt16 = fix->tilt_m+floorf(fix->values[CURRENT][TILT]*fix->tilt_k);
    uint8_t strobeval = 32+floorf(fix->values[CURRENT][STROBE]*(255.0f-32.0f));

    conduit_send(con, fix->base_channel,    pan16>>8);
    conduit_send(con, fix->base_channel+ 1, pan16&255);
    conduit_send(con, fix->base_channel+ 2, tilt16>>8);
    conduit_send(con, fix->base_channel+ 3, tilt16&255);
    conduit_send(con, fix->base_channel+ 4, floorf(fix->values[CURRENT][RED]*255.0f));
    conduit_send(con, fix->base_channel+ 5, floorf(fix->values[CURRENT][GREEN]*255.0f));
    conduit_send(con, fix->base_channel+ 6, floorf(fix->values[CURRENT][BLUE]*255.0f));
    conduit_send(con, fix->base_channel+ 7, floorf(fix->values[CURRENT][WHITE]*255.0f));
    conduit_send(con, fix->base_channel+ 9, floorf(fix->values[CURRENT][ULTRAVIOLET]*255.0f));
    conduit_send(con, fix->base_channel+11, strobeval);
  }
}
