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
void assign_fixture_widget_model_data(model *global, fixture_widget *fw)
{
  assign_model_data(global, &fw->m);
  fw->m.vtx_n = fixture_widget_triangle_count(fw);
  global->vtx_n += fw->m.vtx_n;
}
void initialize_fixture_widget_data(fixture_widget *fw, screen_dim x, screen_dim y, screen_dim w, screen_dim h)
{
  fw->dims.pos[0]  = x;
  fw->dims.pos[1]  = y;
  fw->dims.size[0] = w;
  fw->dims.size[1] = h;
}
