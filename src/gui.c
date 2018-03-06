#include "gui.h"
#include <stdlib.h>
size_t gui_widget_triangle_count(const gui_widget *gw)
{
  switch (gw->type) {
  case INACTIVE:
    return TRI_PER_INACTIVE;
  case BUTTON:
    return TRI_PER_BUTTON;
  case SWATCH2:
    return TRI_PER_SWATCH2;
  case SWATCH4:
    return TRI_PER_SWATCH4;
  default:
    fprintf(stderr, "no such gui widget type: %" PRI_GWT "\n", gw->type);
    exit(1);
  }
}
static void assign_gui_widget_model_data(model *global, gui_widget *gw)
{
  assign_model_data(global, &gw->m);
  gw->m.vtx_n = gui_widget_triangle_count(gw)*VTX_PER_TRI;
  global->vtx_n += gw->m.vtx_n;
}
bool is_inside(const hitbox *h, screen_dim x, screen_dim y)
{
  return x >= h->pos[0] && x < h->pos[0]+h->size[0] &&
    y >= h->pos[1] && y < h->pos[1]+h->size[1];
}
static void initialize_inactive(gui_widget *inactive)
{
  const screen_dim x = inactive->dims.pos[0];
  const screen_dim y = inactive->dims.pos[1];
  const screen_dim w = inactive->dims.size[0];
  const screen_dim h = inactive->dims.size[1];
  const screen_dim m = 3;
  for (size_t i=0; i<TRI_PER_INACTIVE*VTX_PER_TRI; ++i) {
    inactive->m.points[i*DIM_PER_VTX+0] = rand()*100;
    inactive->m.points[i*DIM_PER_VTX+1] = rand()*100;
  }
  for (size_t i=0; i<TRI_PER_INACTIVE*VTX_PER_TRI; ++i) {
    inactive->m.colors[i*CLR_PER_VTX+0] = 0.15f;
    inactive->m.colors[i*CLR_PER_VTX+1] = 0.15f;
    inactive->m.colors[i*CLR_PER_VTX+2] = 0.15f;
  }
  inactive->m.points[ 0] = x;
  inactive->m.points[ 1] = y;
  inactive->m.points[ 2] = x+w;
  inactive->m.points[ 3] = y;
  inactive->m.points[ 4] = x+w;
  inactive->m.points[ 5] = y+h;

  inactive->m.points[ 6] = x;
  inactive->m.points[ 7] = y;
  inactive->m.points[ 8] = x+w;
  inactive->m.points[ 9] = y+h;
  inactive->m.points[10] = x;
  inactive->m.points[11] = y+h;
}
static void initialize_button(gui_widget *button)
{
  button->button.on = false;
  button->button.pressed = false;
  const screen_dim x = button->dims.pos[0];
  const screen_dim y = button->dims.pos[1];
  const screen_dim w = button->dims.size[0];
  const screen_dim h = button->dims.size[1];
  const screen_dim m = 3;
  for (size_t i=0; i<TRI_PER_BUTTON*VTX_PER_TRI; ++i) {
    button->m.points[i*DIM_PER_VTX+0] = rand()*100;
    button->m.points[i*DIM_PER_VTX+1] = rand()*100;
  }
  for (size_t i=0; i<TRI_PER_BUTTON*VTX_PER_TRI; ++i) {
    button->m.colors[i*CLR_PER_VTX+0] = 1.0f;
    button->m.colors[i*CLR_PER_VTX+1] = 0.0f;
    button->m.colors[i*CLR_PER_VTX+2] = 1.0f;
  }

  button->m.points[ 0] = x;
  button->m.points[ 1] = y;
  button->m.points[ 2] = x+w;
  button->m.points[ 3] = y;
  button->m.points[ 4] = x+w;
  button->m.points[ 5] = y+h;

  button->m.points[ 6] = x;
  button->m.points[ 7] = y;
  button->m.points[ 8] = x+w;
  button->m.points[ 9] = y+h;
  button->m.points[10] = x;
  button->m.points[11] = y+h;

  button->m.points[12] = x+m;
  button->m.points[13] = y+m;
  button->m.points[14] = x+w-m;
  button->m.points[15] = y+m;
  button->m.points[16] = x+w-m;
  button->m.points[17] = y+h-m;

  button->m.points[18] = x+m;
  button->m.points[19] = y+m;
  button->m.points[20] = x+w-m;
  button->m.points[21] = y+h-m;
  button->m.points[22] = x+m;
  button->m.points[23] = y+h-m;
}
static void initialize_swatch2(gui_widget *swatch)
{
  const screen_dim x = swatch->dims.pos[0];
  const screen_dim y = swatch->dims.pos[1];
  const screen_dim w = swatch->dims.size[0];
  const screen_dim h = swatch->dims.size[1];
  const screen_dim m = 3;
  for (size_t i=0; i<TRI_PER_SWATCH2*VTX_PER_TRI; ++i) {
    swatch->m.points[i*DIM_PER_VTX+0] = rand()*100;
    swatch->m.points[i*DIM_PER_VTX+1] = rand()*100;
  }
  for (size_t i=0; i<TRI_PER_SWATCH2*VTX_PER_TRI; ++i) {
    swatch->m.colors[i*CLR_PER_VTX+0] = 1.0f;
    swatch->m.colors[i*CLR_PER_VTX+1] = 0.0f;
    swatch->m.colors[i*CLR_PER_VTX+2] = 1.0f;
  }

  swatch->m.points[ 0] = x;
  swatch->m.points[ 1] = y;
  swatch->m.points[ 2] = x+w;
  swatch->m.points[ 3] = y;
  swatch->m.points[ 4] = x+w;
  swatch->m.points[ 5] = y+h;

  swatch->m.points[ 6] = x;
  swatch->m.points[ 7] = y;
  swatch->m.points[ 8] = x+w;
  swatch->m.points[ 9] = y+h;
  swatch->m.points[10] = x;
  swatch->m.points[11] = y+h;

  swatch->m.points[12] = x+m;
  swatch->m.points[13] = y+m;
  swatch->m.points[14] = x+w-m;
  swatch->m.points[15] = y+m;
  swatch->m.points[16] = x+w-m;
  swatch->m.points[17] = y+h-m;

  swatch->m.points[18] = x+m;
  swatch->m.points[19] = y+m;
  swatch->m.points[20] = x+w-m;
  swatch->m.points[21] = y+h-m;
  swatch->m.points[22] = x+m;
  swatch->m.points[23] = y+h-m;
}
static void initialize_swatch4(gui_widget *swatch)
{
  const screen_dim x = swatch->dims.pos[0];
  const screen_dim y = swatch->dims.pos[1];
  const screen_dim w = swatch->dims.size[0];
  const screen_dim h = swatch->dims.size[1];
  const screen_dim m = 3;
  for (size_t i=0; i<TRI_PER_SWATCH4*VTX_PER_TRI; ++i) {
    swatch->m.points[i*DIM_PER_VTX+0] = rand()*100;
    swatch->m.points[i*DIM_PER_VTX+1] = rand()*100;
  }
  for (size_t i=0; i<TRI_PER_SWATCH4*VTX_PER_TRI; ++i) {
    swatch->m.colors[i*CLR_PER_VTX+0] = 0.0f;
    swatch->m.colors[i*CLR_PER_VTX+1] = 1.0f;
    swatch->m.colors[i*CLR_PER_VTX+2] = 0.3f;
  }

  swatch->m.points[ 0] = x;
  swatch->m.points[ 1] = y;
  swatch->m.points[ 2] = x+w;
  swatch->m.points[ 3] = y;
  swatch->m.points[ 4] = x+w;
  swatch->m.points[ 5] = y+h;

  swatch->m.points[ 6] = x;
  swatch->m.points[ 7] = y;
  swatch->m.points[ 8] = x+w;
  swatch->m.points[ 9] = y+h;
  swatch->m.points[10] = x;
  swatch->m.points[11] = y+h;

  swatch->m.points[12] = x+m;
  swatch->m.points[13] = y+m;
  swatch->m.points[14] = x+w-m;
  swatch->m.points[15] = y+m;
  swatch->m.points[16] = x+w/2;
  swatch->m.points[17] = y+h/2;

  swatch->m.points[18] = x+w/2;
  swatch->m.points[19] = y+h/2;
  swatch->m.points[20] = x+w-m;
  swatch->m.points[21] = y+m;
  swatch->m.points[22] = x+w-m;
  swatch->m.points[23] = y+h-m;

  swatch->m.points[24] = x+m;
  swatch->m.points[25] = y+h-m;
  swatch->m.points[26] = x+w/2;
  swatch->m.points[27] = y+h/2;
  swatch->m.points[28] = x+w-m;
  swatch->m.points[29] = y+h-m;

  swatch->m.points[30] = x+m;
  swatch->m.points[31] = y+m;
  swatch->m.points[32] = x+w/2;
  swatch->m.points[33] = y+h/2;
  swatch->m.points[34] = x+m;
  swatch->m.points[35] = y+h-m;
}
void initialize_gui_widget(model *global, gui_widget *gw, screen_dim x, screen_dim y, screen_dim w, screen_dim h, gui_widget_type type)
{
  gw->type = type;
  gw->dims.pos[0]  = x;
  gw->dims.pos[1]  = y;
  gw->dims.size[0] = w;
  gw->dims.size[1] = h;
  gw->hover = false;
  gw->select = false;

  assign_gui_widget_model_data(global, gw);

  switch (type) {
  case INACTIVE:
    initialize_inactive(gw);
    break;
  case BUTTON:
    initialize_button(gw);
    break;
  case SWATCH2:
    initialize_swatch2(gw);
    break;
  case SWATCH4:
    initialize_swatch4(gw);
    break;
  default:
    fprintf(stderr, "no such gui widget type: %" PRI_GWT "\n", type);
    exit(1);
  }
}


static inline void update_button_graphics(gui_widget *button, bool heart)
{
  float r, g, b;
  if (button->hover) {
    r = g = b = 1.0f;
  }
  else if (button->select) {
    r = g = heart ? 1.0f : 0.5f;
    b = 0.0f;
  }
  else {
    r = g = b = 0.0f;
  }
  for (size_t i=0; i<2*VTX_PER_TRI; ++i) {
    button->m.colors[i*CLR_PER_VTX+0] = r;
    button->m.colors[i*CLR_PER_VTX+1] = g;
    button->m.colors[i*CLR_PER_VTX+2] = b;
  }
  if (button->button.pressed) {
    r = g = b = 0.4f;
  }
  else if (button->button.on) {
    r = g = b = 0.6f;
  }
  else {
    r = g = b = 0.0f;
  }
  for (size_t i=2*VTX_PER_TRI; i<4*VTX_PER_TRI; ++i) {
    button->m.colors[i*CLR_PER_VTX+0] = r;
    button->m.colors[i*CLR_PER_VTX+1] = g;
    button->m.colors[i*CLR_PER_VTX+2] = b;
  }
}
static inline void update_swatch2_graphics(gui_widget *swatch, bool heart)
{
  float r, g, b;
  if (swatch->hover) {
    r = g = b = 1.0f;
  }
  else if (swatch->select) {
    r = g = heart ? 1.0f : 0.5f;
    b = 0.0f;
  }
  else {
    r = g = b = 0.0f;
  }

  swatch->m.colors[ 0] = swatch->m.colors[ 3] = swatch->m.colors[ 6] = r;
  swatch->m.colors[ 1] = swatch->m.colors[ 4] = swatch->m.colors[ 7] = g;
  swatch->m.colors[ 2] = swatch->m.colors[ 5] = swatch->m.colors[ 8] = b;
  swatch->m.colors[ 9] = swatch->m.colors[12] = swatch->m.colors[15] = r;
  swatch->m.colors[10] = swatch->m.colors[13] = swatch->m.colors[16] = g;
  swatch->m.colors[11] = swatch->m.colors[14] = swatch->m.colors[17] = b;

  r = 1.0f; g = 0.5f; b=0.0f;

  swatch->m.colors[18] = swatch->m.colors[21] = swatch->m.colors[24] = r;
  swatch->m.colors[19] = swatch->m.colors[22] = swatch->m.colors[25] = g;
  swatch->m.colors[20] = swatch->m.colors[23] = swatch->m.colors[26] = b;

  r = 0.0f; g = 0.0f; b=0.2f;

  swatch->m.colors[27] = swatch->m.colors[30] = swatch->m.colors[33] = r;
  swatch->m.colors[28] = swatch->m.colors[31] = swatch->m.colors[34] = g;
  swatch->m.colors[29] = swatch->m.colors[32] = swatch->m.colors[35] = b;

}
static inline void update_swatch4_graphics(gui_widget *swatch, bool heart)
{
  float r, g, b;
  if (swatch->hover) {
    r = g = b = 1.0f;
  }
  else if (swatch->select) {
    r = g = heart ? 1.0f : 0.5f;
    b = 0.0f;
  }
  else {
    r = g = b = 0.0f;
  }

  swatch->m.colors[ 0] = swatch->m.colors[ 3] = swatch->m.colors[ 6] = r;
  swatch->m.colors[ 1] = swatch->m.colors[ 4] = swatch->m.colors[ 7] = g;
  swatch->m.colors[ 2] = swatch->m.colors[ 5] = swatch->m.colors[ 8] = b;
  swatch->m.colors[ 9] = swatch->m.colors[12] = swatch->m.colors[15] = r;
  swatch->m.colors[10] = swatch->m.colors[13] = swatch->m.colors[16] = g;
  swatch->m.colors[11] = swatch->m.colors[14] = swatch->m.colors[17] = b;

  r = 1.0f; g = 0.0f; b=0.0f;

  swatch->m.colors[18] = swatch->m.colors[21] = swatch->m.colors[24] = r;
  swatch->m.colors[19] = swatch->m.colors[22] = swatch->m.colors[25] = g;
  swatch->m.colors[20] = swatch->m.colors[23] = swatch->m.colors[26] = b;

  r = 1.0f; g = 0.5f; b=0.0f;

  swatch->m.colors[27] = swatch->m.colors[30] = swatch->m.colors[33] = r;
  swatch->m.colors[28] = swatch->m.colors[31] = swatch->m.colors[34] = g;
  swatch->m.colors[29] = swatch->m.colors[32] = swatch->m.colors[35] = b;

  r = 1.0f; g = 1.0f; b=0.0f;

  swatch->m.colors[36] = swatch->m.colors[39] = swatch->m.colors[42] = r;
  swatch->m.colors[37] = swatch->m.colors[40] = swatch->m.colors[43] = g;
  swatch->m.colors[38] = swatch->m.colors[41] = swatch->m.colors[44] = b;

  r = 0.5f; g = 1.0f; b=0.0f;

  swatch->m.colors[45] = swatch->m.colors[48] = swatch->m.colors[51] = r;
  swatch->m.colors[46] = swatch->m.colors[49] = swatch->m.colors[52] = g;
  swatch->m.colors[47] = swatch->m.colors[50] = swatch->m.colors[53] = b;

}


void update_gui_widgets_graphics(gui_widget *gw, size_t n, bool heart)
{
  for (size_t i=0; i<n; ++i) {

    // if (changed) {
    //   changed = false;

    switch (gw[i].type) {
    case INACTIVE:
      // does not need to be updated ever
      break;
    case BUTTON:
      update_button_graphics(&gw[i], heart);
      break;
    case SWATCH2:
      update_swatch2_graphics(&gw[i], heart);
      break;
    case SWATCH4:
      update_swatch4_graphics(&gw[i], heart);
      break;
    default:
      fprintf(stderr, "no such gui widget type: %" PRI_GWT "\n", gw->type);
      exit(1);
    }
    // }
  }
}
