#include "gui.h"
#include <stdlib.h>
size_t gui_widget_triangle_count(const gui_widget *gw)
{
  switch (gw->type) {
  case BUTTON:
    return TRI_PER_BUTTON;
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
    printf("DIM i=%2zu, -> %2zu, %2zu\n", i, i*DIM_PER_VTX+0, i*DIM_PER_VTX+1);
  }
  for (size_t i=0; i<TRI_PER_BUTTON*VTX_PER_TRI; ++i) {
    button->m.colors[i*CLR_PER_VTX+0] = 1.0f;
    button->m.colors[i*CLR_PER_VTX+1] = 0.0f;
    button->m.colors[i*CLR_PER_VTX+2] = 1.0f;
    printf("CLR i=%2zu, -> %2zu, %2zu, %2zu\n", i, i*CLR_PER_VTX+0, i*CLR_PER_VTX+1, i*CLR_PER_VTX+2);
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
  case BUTTON:
    initialize_button(gw);
    break;
  default:
    fprintf(stderr, "no such gui widget type: %" PRI_GWT "\n", type);
    exit(1);
  }
}


void update_button_graphics(gui_widget *button, bool heart)
{
  float r, g, b;
  if (button->hover) {
    r = g = b = 1.0f;
  }
  else if (button->select) {
    r = g = heart ? 1.0f : 0.9f;
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


void update_gui_widget_graphics(gui_widget *gw, bool heart)
{
  switch (gw->type) {
  case BUTTON:
    update_button_graphics(gw, heart);
    break;
  default:
    fprintf(stderr, "no such gui widget type: %" PRI_GWT "\n", gw->type);
    exit(1);
  }
}
