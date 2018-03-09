#include "light.h"
size_t fixture_widget_triangle_count(const fixture_widget *fw)
{
  switch (fw->fix->type) {
  case SINGLECHANNEL:
    return TRI_PER_SINGLECHANNEL;
  case LEDPAR56:
    return TRI_PER_LEDPAR56;
  case LEDWASH:
    return TRI_PER_LEDWASH;
  case LEDMH:
    return TRI_PER_LEDMH;
  case LEDMH2:
    return TRI_PER_LEDMH2;
  }
  return 0;
}
void assign_fixture_widget_model_data(model *global, fixture_widget *fw)
{
  assign_model_data(global, &fw->m);
  fw->m.vtx_n = fixture_widget_triangle_count(fw)*VTX_PER_TRI;
  global->vtx_n += fw->m.vtx_n;
}
static void initialize_singlechannel_widget(fixture_widget *fw)
{
  const screen_dim x = fw->dims.pos[0];
  const screen_dim y = fw->dims.pos[1];
  const screen_dim w = fw->dims.size[0];
  const screen_dim h = fw->dims.size[1];

  fw->m.points[ 0] = x;
  fw->m.points[ 1] = y;
  fw->m.points[ 2] = x+w;
  fw->m.points[ 3] = y;
  fw->m.points[ 4] = x+w;
  fw->m.points[ 5] = y+h;

  fw->m.points[ 6] = x;
  fw->m.points[ 7] = y;
  fw->m.points[ 8] = x+w;
  fw->m.points[ 9] = y+h;
  fw->m.points[10] = x;
  fw->m.points[11] = y+h;

  for (size_t i=0; i<TRI_PER_SINGLECHANNEL*VTX_PER_TRI; ++i) {
    fw->m.colors[i*CLR_PER_VTX+0] = 0.0f;
    fw->m.colors[i*CLR_PER_VTX+1] = 0.0f;
    fw->m.colors[i*CLR_PER_VTX+2] = 0.0f;
  }
}
static void initialize_ledpar56_widget(fixture_widget *fw)
{
  initialize_singlechannel_widget(fw);
}
static void initialize_ledwash_widget(fixture_widget *fw)
{
  const screen_dim u = fw->dims.size[0]/4;
  // main
  {
    const screen_dim x = fw->dims.pos[0];
    const screen_dim y = fw->dims.pos[1];
    const screen_dim w = u*3;
    const screen_dim h = u*2;

    fw->m.points[ 0] = x;
    fw->m.points[ 1] = y;
    fw->m.points[ 2] = x+w;
    fw->m.points[ 3] = y;
    fw->m.points[ 4] = x+w;
    fw->m.points[ 5] = y+h;

    fw->m.points[ 6] = x;
    fw->m.points[ 7] = y;
    fw->m.points[ 8] = x+w;
    fw->m.points[ 9] = y+h;
    fw->m.points[10] = x;
    fw->m.points[11] = y+h;
  }
  // white
  {
    const screen_dim x = fw->dims.pos[0] + u*3;
    const screen_dim y = fw->dims.pos[1];
    const screen_dim w = u;
    const screen_dim h = u;

    fw->m.points[12] = x;
    fw->m.points[13] = y;
    fw->m.points[14] = x+w;
    fw->m.points[15] = y;
    fw->m.points[16] = x+w;
    fw->m.points[17] = y+h;

    fw->m.points[18] = x;
    fw->m.points[19] = y;
    fw->m.points[20] = x+w;
    fw->m.points[21] = y+h;
    fw->m.points[22] = x;
    fw->m.points[23] = y+h;
  }
  // strobe
  {
    const screen_dim x = fw->dims.pos[0] + u*3;
    const screen_dim y = fw->dims.pos[1] + u;
    const screen_dim w = u;
    const screen_dim h = u;

    fw->m.points[24] = x;
    fw->m.points[25] = y;
    fw->m.points[26] = x+w;
    fw->m.points[27] = y;
    fw->m.points[28] = x+w;
    fw->m.points[29] = y+h;

    fw->m.points[30] = x;
    fw->m.points[31] = y;
    fw->m.points[32] = x+w;
    fw->m.points[33] = y+h;
    fw->m.points[34] = x;
    fw->m.points[35] = y+h;
  }

  for (size_t i=0; i<TRI_PER_LEDWASH*VTX_PER_TRI; ++i) {
    fw->m.colors[i*CLR_PER_VTX+0] = 0.0f;
    fw->m.colors[i*CLR_PER_VTX+1] = 0.0f;
    fw->m.colors[i*CLR_PER_VTX+2] = 0.0f;
  }

}
static void initialize_ledmh_widget(fixture_widget *fw)
{
  const screen_dim u = fw->dims.size[0]/4;
  // main
  {
    const screen_dim x = fw->dims.pos[0];
    const screen_dim y = fw->dims.pos[1];
    const screen_dim w = u*3;
    const screen_dim h = u*3;

    fw->m.points[ 0] = x;
    fw->m.points[ 1] = y;
    fw->m.points[ 2] = x+w;
    fw->m.points[ 3] = y;
    fw->m.points[ 4] = x+w;
    fw->m.points[ 5] = y+h;

    fw->m.points[ 6] = x;
    fw->m.points[ 7] = y;
    fw->m.points[ 8] = x+w;
    fw->m.points[ 9] = y+h;
    fw->m.points[10] = x;
    fw->m.points[11] = y+h;
  }
  // white
  {
    const screen_dim x = fw->dims.pos[0] + u*3;
    const screen_dim y = fw->dims.pos[1];
    const screen_dim w = u;
    const screen_dim h = u;

    fw->m.points[12] = x;
    fw->m.points[13] = y;
    fw->m.points[14] = x+w;
    fw->m.points[15] = y;
    fw->m.points[16] = x+w;
    fw->m.points[17] = y+h;

    fw->m.points[18] = x;
    fw->m.points[19] = y;
    fw->m.points[20] = x+w;
    fw->m.points[21] = y+h;
    fw->m.points[22] = x;
    fw->m.points[23] = y+h;
  }
  // strobe
  {
    const screen_dim x = fw->dims.pos[0] + u*3;
    const screen_dim y = fw->dims.pos[1] + u;
    const screen_dim w = u;
    const screen_dim h = u;

    fw->m.points[24] = x;
    fw->m.points[25] = y;
    fw->m.points[26] = x+w;
    fw->m.points[27] = y;
    fw->m.points[28] = x+w;
    fw->m.points[29] = y+h;

    fw->m.points[30] = x;
    fw->m.points[31] = y;
    fw->m.points[32] = x+w;
    fw->m.points[33] = y+h;
    fw->m.points[34] = x;
    fw->m.points[35] = y+h;
  }
  // pan/tilt indicator
  {
    const screen_dim x = fw->dims.pos[0];
    const screen_dim y = fw->dims.pos[1];
    const screen_dim w = u;
    const screen_dim h = u;

    fw->m.points[36] = x;
    fw->m.points[37] = y;
    fw->m.points[38] = x+w;
    fw->m.points[39] = y;
    fw->m.points[40] = x+w;
    fw->m.points[41] = y+h;

    fw->m.points[42] = x;
    fw->m.points[43] = y;
    fw->m.points[44] = x+w;
    fw->m.points[45] = y+h;
    fw->m.points[46] = x;
    fw->m.points[47] = y+h;
  }
  for (size_t i=0; i<TRI_PER_LEDMH*VTX_PER_TRI; ++i) {
    fw->m.colors[i*CLR_PER_VTX+0] = 0.0f;
    fw->m.colors[i*CLR_PER_VTX+1] = 0.0f;
    fw->m.colors[i*CLR_PER_VTX+2] = 0.0f;
  }

}
static void initialize_ledmh2_widget(fixture_widget *fw)
{
  const screen_dim u = fw->dims.size[0]/4;
  // main
  {
    const screen_dim x = fw->dims.pos[0];
    const screen_dim y = fw->dims.pos[1];
    const screen_dim w = u*3;
    const screen_dim h = u*3;

    fw->m.points[ 0] = x;
    fw->m.points[ 1] = y;
    fw->m.points[ 2] = x+w;
    fw->m.points[ 3] = y;
    fw->m.points[ 4] = x+w;
    fw->m.points[ 5] = y+h;

    fw->m.points[ 6] = x;
    fw->m.points[ 7] = y;
    fw->m.points[ 8] = x+w;
    fw->m.points[ 9] = y+h;
    fw->m.points[10] = x;
    fw->m.points[11] = y+h;
  }
  // white
  {
    const screen_dim x = fw->dims.pos[0] + u*3;
    const screen_dim y = fw->dims.pos[1];
    const screen_dim w = u;
    const screen_dim h = u;

    fw->m.points[12] = x;
    fw->m.points[13] = y;
    fw->m.points[14] = x+w;
    fw->m.points[15] = y;
    fw->m.points[16] = x+w;
    fw->m.points[17] = y+h;

    fw->m.points[18] = x;
    fw->m.points[19] = y;
    fw->m.points[20] = x+w;
    fw->m.points[21] = y+h;
    fw->m.points[22] = x;
    fw->m.points[23] = y+h;
  }
  // strobe
  {
    const screen_dim x = fw->dims.pos[0] + u*3;
    const screen_dim y = fw->dims.pos[1] + u;
    const screen_dim w = u;
    const screen_dim h = u;

    fw->m.points[24] = x;
    fw->m.points[25] = y;
    fw->m.points[26] = x+w;
    fw->m.points[27] = y;
    fw->m.points[28] = x+w;
    fw->m.points[29] = y+h;

    fw->m.points[30] = x;
    fw->m.points[31] = y;
    fw->m.points[32] = x+w;
    fw->m.points[33] = y+h;
    fw->m.points[34] = x;
    fw->m.points[35] = y+h;
  }
  // uv
  {
    const screen_dim x = fw->dims.pos[0] + u*3;
    const screen_dim y = fw->dims.pos[1] + u*2;
    const screen_dim w = u;
    const screen_dim h = u;

    fw->m.points[36] = x;
    fw->m.points[37] = y;
    fw->m.points[38] = x+w;
    fw->m.points[39] = y;
    fw->m.points[40] = x+w;
    fw->m.points[41] = y+h;

    fw->m.points[42] = x;
    fw->m.points[43] = y;
    fw->m.points[44] = x+w;
    fw->m.points[45] = y+h;
    fw->m.points[46] = x;
    fw->m.points[47] = y+h;
  }
  // pan/tilt indicator
  {
    const screen_dim x = fw->dims.pos[0];
    const screen_dim y = fw->dims.pos[1];
    const screen_dim w = u;
    const screen_dim h = u;

    fw->m.points[48] = x;
    fw->m.points[49] = y;
    fw->m.points[50] = x+w;
    fw->m.points[51] = y;
    fw->m.points[52] = x+w;
    fw->m.points[53] = y+h;

    fw->m.points[54] = x;
    fw->m.points[55] = y;
    fw->m.points[56] = x+w;
    fw->m.points[57] = y+h;
    fw->m.points[58] = x;
    fw->m.points[59] = y+h;
  }
  for (size_t i=0; i<TRI_PER_LEDMH2*VTX_PER_TRI; ++i) {
    fw->m.colors[i*CLR_PER_VTX+0] = 0.0f;
    fw->m.colors[i*CLR_PER_VTX+1] = 0.0f;
    fw->m.colors[i*CLR_PER_VTX+2] = 0.0f;
  }

}
void initialize_fixture_widget(model *global, fixture_widget *fw, fixture *fix, screen_dim x, screen_dim y, screen_dim w, screen_dim h)
{
  fw->dims.pos[0]  = x;
  fw->dims.pos[1]  = y;
  fw->dims.size[0] = w;
  fw->dims.size[1] = h;
  fw->fix = fix;
  assign_fixture_widget_model_data(global, fw);
  switch (fw->fix->type) {
  case SINGLECHANNEL:
    initialize_singlechannel_widget(fw);
    break;
  case LEDPAR56:
    initialize_ledpar56_widget(fw);
    break;
  case LEDWASH:
    initialize_ledwash_widget(fw);
    break;
  case LEDMH:
    initialize_ledmh_widget(fw);
    break;
  case LEDMH2:
    initialize_ledmh2_widget(fw);
    break;
  }
}

static inline void update_singlechannel_widget(fixture_widget *fw)
{
  GLfloat red = 1.0f;
  GLfloat green = 0.4f;
  GLfloat blue = 0.0f;

  fw->m.colors[ 0] = fw->m.colors[ 3] = fw->m.colors[ 6] = red;
  fw->m.colors[ 1] = fw->m.colors[ 4] = fw->m.colors[ 7] = green;
  fw->m.colors[ 2] = fw->m.colors[ 5] = fw->m.colors[ 8] = blue;
  fw->m.colors[ 9] = fw->m.colors[12] = fw->m.colors[15] = red;
  fw->m.colors[10] = fw->m.colors[13] = fw->m.colors[16] = green;
  fw->m.colors[11] = fw->m.colors[14] = fw->m.colors[17] = blue;
}
static inline void update_ledpar56_widget(fixture_widget *fw)
{
  GLfloat red = 0.1f;
  GLfloat green = 1.0f;
  GLfloat blue = 0.0f;

  fw->m.colors[ 0] = fw->m.colors[ 3] = fw->m.colors[ 6] = red;
  fw->m.colors[ 1] = fw->m.colors[ 4] = fw->m.colors[ 7] = green;
  fw->m.colors[ 2] = fw->m.colors[ 5] = fw->m.colors[ 8] = blue;
  fw->m.colors[ 9] = fw->m.colors[12] = fw->m.colors[15] = red;
  fw->m.colors[10] = fw->m.colors[13] = fw->m.colors[16] = green;
  fw->m.colors[11] = fw->m.colors[14] = fw->m.colors[17] = blue;
}
static inline void update_ledwash_widget(fixture_widget *fw)
{
  GLfloat red = 0.0f;
  GLfloat green = 0.3f;
  GLfloat blue = 1.0f;
  GLfloat white = 0.3f;
  GLfloat strobe0 = 0.0f;
  GLfloat strobe1 = 1.0f;

  fw->m.colors[ 0] = fw->m.colors[ 3] = fw->m.colors[ 6] = red;
  fw->m.colors[ 1] = fw->m.colors[ 4] = fw->m.colors[ 7] = green;
  fw->m.colors[ 2] = fw->m.colors[ 5] = fw->m.colors[ 8] = blue;
  fw->m.colors[ 9] = fw->m.colors[12] = fw->m.colors[15] = red;
  fw->m.colors[10] = fw->m.colors[13] = fw->m.colors[16] = green;
  fw->m.colors[11] = fw->m.colors[14] = fw->m.colors[17] = blue;

  fw->m.colors[18] = fw->m.colors[21] = fw->m.colors[24] = white;
  fw->m.colors[19] = fw->m.colors[22] = fw->m.colors[25] = white;
  fw->m.colors[20] = fw->m.colors[23] = fw->m.colors[26] = white;
  fw->m.colors[27] = fw->m.colors[30] = fw->m.colors[33] = white;
  fw->m.colors[28] = fw->m.colors[31] = fw->m.colors[34] = white;
  fw->m.colors[29] = fw->m.colors[32] = fw->m.colors[35] = white;

  fw->m.colors[36] = fw->m.colors[39] = fw->m.colors[42] = strobe0;
  fw->m.colors[37] = fw->m.colors[40] = fw->m.colors[43] = strobe0;
  fw->m.colors[38] = fw->m.colors[41] = fw->m.colors[44] = strobe0;
  fw->m.colors[45] = fw->m.colors[48] = fw->m.colors[51] = strobe1;
  fw->m.colors[46] = fw->m.colors[49] = fw->m.colors[52] = strobe1;
  fw->m.colors[47] = fw->m.colors[50] = fw->m.colors[53] = strobe1;
}
static inline void update_ledmh_widget(fixture_widget *fw)
{
  GLfloat red = 0.1f;
  GLfloat green = 0.3f;
  GLfloat blue = 0.1f;
  GLfloat white = 0.3f;
  GLfloat strobe0 = 0.0f;
  GLfloat strobe1 = 1.0f;
  GLfloat pt_clr = red+green+blue < 1.0f ? 1.0f : 0.0f;

  GLfloat pan = 0.5;
  GLfloat tilt = 0.5;

  fw->m.colors[ 0] = fw->m.colors[ 3] = fw->m.colors[ 6] = red;
  fw->m.colors[ 1] = fw->m.colors[ 4] = fw->m.colors[ 7] = green;
  fw->m.colors[ 2] = fw->m.colors[ 5] = fw->m.colors[ 8] = blue;
  fw->m.colors[ 9] = fw->m.colors[12] = fw->m.colors[15] = red;
  fw->m.colors[10] = fw->m.colors[13] = fw->m.colors[16] = green;
  fw->m.colors[11] = fw->m.colors[14] = fw->m.colors[17] = blue;

  fw->m.colors[18] = fw->m.colors[21] = fw->m.colors[24] = white;
  fw->m.colors[19] = fw->m.colors[22] = fw->m.colors[25] = white;
  fw->m.colors[20] = fw->m.colors[23] = fw->m.colors[26] = white;
  fw->m.colors[27] = fw->m.colors[30] = fw->m.colors[33] = white;
  fw->m.colors[28] = fw->m.colors[31] = fw->m.colors[34] = white;
  fw->m.colors[29] = fw->m.colors[32] = fw->m.colors[35] = white;

  fw->m.colors[36] = fw->m.colors[39] = fw->m.colors[42] = strobe0;
  fw->m.colors[37] = fw->m.colors[40] = fw->m.colors[43] = strobe0;
  fw->m.colors[38] = fw->m.colors[41] = fw->m.colors[44] = strobe0;
  fw->m.colors[45] = fw->m.colors[48] = fw->m.colors[51] = strobe1;
  fw->m.colors[46] = fw->m.colors[49] = fw->m.colors[52] = strobe1;
  fw->m.colors[47] = fw->m.colors[50] = fw->m.colors[53] = strobe1;

  fw->m.colors[54] = fw->m.colors[57] = fw->m.colors[60] = pt_clr;
  fw->m.colors[55] = fw->m.colors[58] = fw->m.colors[61] = pt_clr;
  fw->m.colors[56] = fw->m.colors[59] = fw->m.colors[62] = pt_clr;
  fw->m.colors[63] = fw->m.colors[66] = fw->m.colors[69] = pt_clr;
  fw->m.colors[64] = fw->m.colors[67] = fw->m.colors[70] = pt_clr;
  fw->m.colors[65] = fw->m.colors[68] = fw->m.colors[71] = pt_clr;

  // pan/tilt indicator
  {
    const screen_dim u = fw->dims.size[0]/4;
    const screen_dim x = fw->dims.pos[0]+(pan+1.0f)*u*0.5f;
    const screen_dim y = fw->dims.pos[1]+(tilt+1.0f)*u*0.5f;
    const screen_dim w = u;
    const screen_dim h = u;

    fw->m.points[36] = x;
    fw->m.points[37] = y;
    fw->m.points[38] = x+w;
    fw->m.points[39] = y;
    fw->m.points[40] = x+w;
    fw->m.points[41] = y+h;

    fw->m.points[42] = x;
    fw->m.points[43] = y;
    fw->m.points[44] = x+w;
    fw->m.points[45] = y+h;
    fw->m.points[46] = x;
    fw->m.points[47] = y+h;
  }
}
static inline void update_ledmh2_widget(fixture_widget *fw)
{
  GLfloat red = 0.1f;
  GLfloat green = 0.3f;
  GLfloat blue = 1.0f;
  GLfloat white = 0.3f;
  GLfloat strobe0 = 0.0f;
  GLfloat strobe1 = 1.0f;
  GLfloat uv = 1.0f;
  GLfloat uvr = 0.3*uv;
  GLfloat uvb = uv;

  GLfloat pan = -1.0f;
  GLfloat tilt = -1.0f;

  GLfloat pt_clr = red+green+blue < 1.0f ? 1.0f : 0.0f;

  fw->m.colors[ 0] = fw->m.colors[ 3] = fw->m.colors[ 6] = red;
  fw->m.colors[ 1] = fw->m.colors[ 4] = fw->m.colors[ 7] = green;
  fw->m.colors[ 2] = fw->m.colors[ 5] = fw->m.colors[ 8] = blue;
  fw->m.colors[ 9] = fw->m.colors[12] = fw->m.colors[15] = red;
  fw->m.colors[10] = fw->m.colors[13] = fw->m.colors[16] = green;
  fw->m.colors[11] = fw->m.colors[14] = fw->m.colors[17] = blue;

  fw->m.colors[18] = fw->m.colors[21] = fw->m.colors[24] = white;
  fw->m.colors[19] = fw->m.colors[22] = fw->m.colors[25] = white;
  fw->m.colors[20] = fw->m.colors[23] = fw->m.colors[26] = white;
  fw->m.colors[27] = fw->m.colors[30] = fw->m.colors[33] = white;
  fw->m.colors[28] = fw->m.colors[31] = fw->m.colors[34] = white;
  fw->m.colors[29] = fw->m.colors[32] = fw->m.colors[35] = white;

  fw->m.colors[36] = fw->m.colors[39] = fw->m.colors[42] = strobe0;
  fw->m.colors[37] = fw->m.colors[40] = fw->m.colors[43] = strobe0;
  fw->m.colors[38] = fw->m.colors[41] = fw->m.colors[44] = strobe0;
  fw->m.colors[45] = fw->m.colors[48] = fw->m.colors[51] = strobe1;
  fw->m.colors[46] = fw->m.colors[49] = fw->m.colors[52] = strobe1;
  fw->m.colors[47] = fw->m.colors[50] = fw->m.colors[53] = strobe1;

  fw->m.colors[54] = fw->m.colors[57] = fw->m.colors[60] = uvr;
  fw->m.colors[55] = fw->m.colors[58] = fw->m.colors[61] = 0.0f;
  fw->m.colors[56] = fw->m.colors[59] = fw->m.colors[62] = uvb;
  fw->m.colors[63] = fw->m.colors[66] = fw->m.colors[69] = uvr;
  fw->m.colors[64] = fw->m.colors[67] = fw->m.colors[70] = 0.0f;
  fw->m.colors[65] = fw->m.colors[68] = fw->m.colors[71] = uvb;

  fw->m.colors[72] = fw->m.colors[75] = fw->m.colors[78] = pt_clr;
  fw->m.colors[73] = fw->m.colors[76] = fw->m.colors[79] = pt_clr;
  fw->m.colors[74] = fw->m.colors[77] = fw->m.colors[80] = pt_clr;
  fw->m.colors[81] = fw->m.colors[84] = fw->m.colors[87] = pt_clr;
  fw->m.colors[82] = fw->m.colors[85] = fw->m.colors[88] = pt_clr;
  fw->m.colors[83] = fw->m.colors[86] = fw->m.colors[89] = pt_clr;

  fw->m.colors[91] = fw->m.colors[94] = fw->m.colors[97] = 1.0f;
  fw->m.colors[92] = fw->m.colors[95] = fw->m.colors[98] = 1.0f;
  fw->m.colors[93] = fw->m.colors[96] = fw->m.colors[99] = 1.0f;
  // pan/tilt indicator
  {
    const screen_dim u = fw->dims.size[0]/4;
    const screen_dim x = fw->dims.pos[0]+(pan+1.0f)*u;
    const screen_dim y = fw->dims.pos[1]+(tilt+1.0f)*u;
    const screen_dim w = u;
    const screen_dim h = u;

    fw->m.points[48] = x;
    fw->m.points[49] = y;
    fw->m.points[50] = x+w;
    fw->m.points[51] = y;
    fw->m.points[52] = x+w;
    fw->m.points[53] = y+h;

    fw->m.points[54] = x;
    fw->m.points[55] = y;
    fw->m.points[56] = x+w;
    fw->m.points[57] = y+h;
    fw->m.points[58] = x;
    fw->m.points[59] = y+h;
  }
}
void update_fixture_widgets(fixture_widget *fws, size_t n)
{
  for (size_t i=0; i<n; ++i) {
    switch (fws[i].fix->type) {
    case SINGLECHANNEL:
      update_singlechannel_widget(&fws[i]);
      break;
    case LEDPAR56:
      update_ledpar56_widget(&fws[i]);
      break;
    case LEDWASH:
      update_ledwash_widget(&fws[i]);
      break;
    case LEDMH:
      update_ledmh_widget(&fws[i]);
      break;
    case LEDMH2:
      update_ledmh2_widget(&fws[i]);
      break;
    }
  }
}
static inline void hue_interpolation(soft_value *out, soft_value start, soft_value stop, float t)
{
  float he = stop  + 3600.0f;
  float hs = start + 3600.0f;
  float ho = 0.0f;
  if (he >= hs) {
    ho = t*he + (1.0f-t)*hs;
  }
  else {
    if (hs-he <= he+360.0f-hs) {
      ho = t*he + (1.0f-t)*(hs+360.0f);
    }
    else {
      ho = t*(he+360.0f) + (1.0f-t)*hs;
    }
  }
  *out = fmodf(ho, 360.0f);


}
static inline void linear_interpolation(soft_value *out, soft_value start, soft_value stop, float t)
{
	*out = t*stop + (1.0f-t)*start;
}
void interpolate_and_map_values(fixture *fix, float t)
{
  hue_interpolation(   &fix->values[CURRENT][HUE],         fix->values[START][HUE],         fix->values[END][HUE],         t);
  linear_interpolation(&fix->values[CURRENT][SATURATION],  fix->values[START][SATURATION],  fix->values[END][SATURATION],  t);
  linear_interpolation(&fix->values[CURRENT][VALUE],       fix->values[START][VALUE],       fix->values[END][VALUE],       t);
  linear_interpolation(&fix->values[CURRENT][WHITE],       fix->values[START][WHITE],       fix->values[END][WHITE],       t);
  linear_interpolation(&fix->values[CURRENT][STROBE],      fix->values[START][STROBE],      fix->values[END][STROBE],      t);
  linear_interpolation(&fix->values[CURRENT][ULTRAVIOLET], fix->values[START][ULTRAVIOLET], fix->values[END][ULTRAVIOLET], t);
  linear_interpolation(&fix->values[CURRENT][PAN],         fix->values[START][PAN],         fix->values[END][PAN],         t);
  linear_interpolation(&fix->values[CURRENT][TILT],        fix->values[START][TILT],        fix->values[END][TILT],        t);
  linear_interpolation(&fix->values[CURRENT][ZOOM],        fix->values[START][ZOOM],        fix->values[END][ZOOM],        t);

  hsv2rgb(&fix->values[CURRENT][RED], &fix->values[CURRENT][GREEN],     &fix->values[CURRENT][BLUE],
          fix->values[CURRENT][HUE],  fix->values[CURRENT][SATURATION], fix->values[CURRENT][VALUE]);
}
