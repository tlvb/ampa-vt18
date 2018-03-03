#include "light.h"
size_t fixture_widget_triangle_count(const fixture_widget *fw)
{
  switch (fw->fix->type) {
  case SINGLECHANNEL:
    return 2;
  case LEDPAR56:
    return 2;
  case LEDWASH:
    return 6;
  case LEDMH:
    return 8;
  case LEDMH2:
    return 10;
  }
  return 0;
}
size_t gui_widget_triangle_count(const gui_widget *gw)
{
  return 2;
}
void assign_model_data(model *global, model *widget_data)
{
  widget_data->points = global->points;
  widget_data->colors = global->colors;
}
void assign_fixture_widget_model_data(model *global, fixture_widget *fw)
{
  assign_model_data(global, &fw->m);
  fw->m.vtx_n = fixture_widget_triangle_count(fw);
  global->vtx_n += fw->m.vtx_n;
}
void assign_gui_widget_model_data(model *global, gui_widget *gw)
{
  assign_model_data(global, &gw->m);
  gw->m.vtx_n = gui_widget_triangle_count(gw);
  global->vtx_n += gw->m.vtx_n;
}
void initialize_fixture_widget_data(fixture_widget *fw, screen_dim x, screen_dim y, screen_dim w, screen_dim h)
{
  fw->dims.pos[0]  = x;
  fw->dims.pos[1]  = y;
  fw->dims.size[0] = w;
  fw->dims.size[1] = h;
}
