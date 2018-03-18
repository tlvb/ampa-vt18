#include "bank.h"

void initialize_bank(property_bank *b)
{
  b->bpm = 60;
  b->on = false;
  b->mask = 8;
  b->in_colors.hueavg = 0.0f;
  b->in_colors.huewindow = 0.0f;
  b->in_colors.valmax = 1.0f;
  b->in_colors.valwindow = 0.5f;
  b->hold_colors.hueavg = 120.0f;
  b->hold_colors.huewindow = 0.0f;
  b->hold_colors.valmax = 1.0f;
  b->hold_colors.valwindow = 0.5f;
  b->out_colors.hueavg = 240.0f;
  b->out_colors.huewindow = 0.0f;
  b->out_colors.valmax = 1.0f;
  b->out_colors.valwindow = 0.5f;
  b->white = false;
  b->strobe_amount = 1.0f;
  b->uv_amount = 0.0f;
  b->zoom = 0.0f;
  b->pattern = 0;
}
void update_bank_bpm(property_bank *b, gui_widget *gw, int16_t delta)
{
  if (delta < 0 || b->bpm > (uint32_t)delta) {
    b->bpm -= delta;
  }
  else {
    b->bpm = 1;
  }
  if (b->bpm > 1920) {
    b->bpm = 1920;
  }
  gw->digits.number = b->bpm;
  b->period = 60000.0f / b->bpm;
}
void update_bank_onoff(property_bank *b, gui_widget *gw)
{
  if (gw->button.on) {
    b->on = true;
  }
  else {
    b->on = false;
  }
}
void update_bank_maskn(property_bank *b, gui_widget *gw, int16_t delta)
{
  if (delta < 0 || b->mask > (uint32_t)delta) {
    b->mask -= delta;
  }
  else {
    b->mask = 0;
  }
  if (b->mask > 99) {
    b->mask = 99;
  }
  gw->digits.number = b->mask;
}
void update_bank_hsv(property_bank *b, gui_widget *gw, bank_index id, int16_t delta, rangespec rs)
{

  color_range *rr[3] = { &b->in_colors, &b->hold_colors, &b->out_colors };
  color_range *r     = rr[id-BANK_INHSV];
  switch (rs) {
  case BANK_HUEAVG:
    r->hueavg = fmodf(r->hueavg+3600.0f+delta*6, 360.0f);
    break;
  case BANK_HUEWINDOW:
    r->huewindow = fmaxf(fminf(r->huewindow+delta*3, 120.0f), 0.0f);
    break;
  case BANK_VALMAX:
    r->valmax = fmaxf(fminf(r->valmax+delta/10.0f, 1.0f), 0.0f);
    break;
  case BANK_VALWINDOW:
    r->valwindow = fmaxf(fminf(r->valwindow+delta/5.0f, 1.0f), 0.0f);
    break;
  }
  float white = b->white ? 1.0f : 0.0f;
  hsv2rgb(&gw->swatch.red[0], &gw->swatch.green[0], &gw->swatch.blue[0], r->hueavg, 1.0f-white, r->valmax);
  hsv2rgb(&gw->swatch.red[1], &gw->swatch.green[1], &gw->swatch.blue[1], fmodf(r->hueavg+r->huewindow+3600.0f, 360.0f), 1.0f-white, 0.5f+r->valmax/2.0f);
  hsv2rgb(&gw->swatch.red[2], &gw->swatch.green[2], &gw->swatch.blue[2], r->hueavg, 1.0f-white, r->valmax*(1.0f-r->valwindow));
  hsv2rgb(&gw->swatch.red[3], &gw->swatch.green[3], &gw->swatch.blue[3], fmodf(r->hueavg-r->huewindow+3600.0f, 360.0f), 1.0f-white, 0.5f+r->valmax/2.0f);
}
void update_bank_white(property_bank *b, gui_widget *gw)
{
  b->white = gw->button.on;
}
void update_bank_strobe(property_bank *b, gui_widget *gw)
{
  if (gw->button.on) {
    b->strobe = 1.0f;
  }
  else {
    b->strobe = 0.0f;
  }
}
void update_bank_strobeamount(property_bank *b, gui_widget *gw, int16_t delta)
{
  b->strobe_amount = fmaxf(fminf(b->strobe_amount+delta/20.0f, 1.0f), 0.0f);
  gw->swatch.red[0] = gw->swatch.green[0] = gw->swatch.blue[0] = 0.5f+b->strobe_amount/2.0f;
  gw->swatch.red[1] = gw->swatch.green[1] = gw->swatch.blue[1] = 0.5f-b->strobe_amount/2.0f;
}
void update_bank_uvamount(property_bank *b, gui_widget *gw, int16_t delta)
{
  b->uv_amount = fmaxf(fminf(b->uv_amount+delta/10.0f, 1.0f), 0.0f);
  gw->swatch.red[0] = b->uv_amount*0.4;
  gw->swatch.green[0] = 0.0f;
  gw->swatch.blue[0] = b->uv_amount;
  gw->swatch.red[1] = b->uv_amount*0.2;
  gw->swatch.green[1] = 0.0f;
  gw->swatch.blue[1] = b->uv_amount*0.5;
}
void update_bank_zoom(property_bank *b, gui_widget *gw, int16_t delta)
{
  if (delta < 0 || b->zoom > (uint32_t)delta) {
    b->zoom -= delta;
  }
  else {
    b->zoom = 0;
  }
  if (b->zoom > 99) {
    b->zoom = 99;
  }
  gw->digits.number = b->zoom;
}
void update_bank_pattern(property_bank *b, gui_widget *gw, int16_t delta)
{
  if (delta < 0 || b->mask > (uint32_t)delta) {
    b->pattern -= delta;
  }
  else {
    b->pattern = 0;
  }
  if (b->pattern > 99) {
    b->pattern = 99;
  }
  gw->digits.number = b->pattern;
}

void clone_bank_bpm(property_bank *tb, gui_widget *tgw, property_bank *sb, gui_widget *sgw)
{
  tb->bpm = sb->bpm;
  tgw->digits.number = sgw->digits.number;
  tb->period = sb->period;
}
void clone_bank_mask(property_bank *tb, gui_widget *tgw, property_bank *sb, gui_widget *sgw)
{
  tb->mask = sb->mask;
  tgw->digits.number = sgw->digits.number;
}
void clone_bank_hsv(property_bank *tb, gui_widget *tgw, bank_index tid, property_bank *sb, gui_widget *sgw, bank_index sid)
{
  color_range *trr[3] = { &tb->in_colors, &tb->hold_colors, &tb->out_colors };
  color_range *srr[3] = { &sb->in_colors, &sb->hold_colors, &sb->out_colors };
  color_range *tr = trr[tid];
  color_range *sr = srr[sid];
  tr->hueavg = sr->hueavg;
  tr->huewindow = sr->huewindow;
  tr->valmax = sr->valmax;
  tr->valwindow = sr->valwindow;
  float white = tb->white ? 1.0f : 0.0f;
  hsv2rgb(&tgw->swatch.red[0], &tgw->swatch.green[0], &tgw->swatch.blue[0], tr->hueavg, 1.0f-white, tr->valmax);
  hsv2rgb(&tgw->swatch.red[1], &tgw->swatch.green[1], &tgw->swatch.blue[1], fmodf(tr->hueavg+tr->huewindow+3600.0f, 360.0f), 1.0f-white, 0.5f+tr->valmax/2.0f);
  hsv2rgb(&tgw->swatch.red[2], &tgw->swatch.green[2], &tgw->swatch.blue[2], tr->hueavg, 1.0f-white, tr->valmax*(1.0f-tr->valwindow));
  hsv2rgb(&tgw->swatch.red[3], &tgw->swatch.green[3], &tgw->swatch.blue[3], fmodf(tr->hueavg-tr->huewindow+3600.0f, 360.0f), 1.0f-white, 0.5f+tr->valmax/2.0f);
}
void clone_bank_strobeamount(property_bank *tb, gui_widget *tgw, property_bank *sb, gui_widget *sgw)
{
  tb->strobe_amount = sb->strobe_amount;
  tgw->swatch.red[0] = tgw->swatch.green[0] = tgw->swatch.blue[0] = 0.5f+tb->strobe_amount/2.0f;
  tgw->swatch.red[1] = tgw->swatch.green[1] = tgw->swatch.blue[1] = 0.5f-tb->strobe_amount/2.0f;
}
void clone_bank_uvamount(property_bank *tb, gui_widget *tgw, property_bank *sb, gui_widget *sgw)
{
  tb->uv_amount = sb->uv_amount;
  tgw->swatch.red[0] = tb->uv_amount*0.4;
  tgw->swatch.green[0] = 0.0f;
  tgw->swatch.blue[0] = tb->uv_amount;
  tgw->swatch.red[1] = tb->uv_amount*0.2;
  tgw->swatch.green[1] = 0.0f;
  tgw->swatch.blue[1] = tb->uv_amount*0.5;
}
void clone_zoom(property_bank *tb, gui_widget *tgw, property_bank *sb, gui_widget *sgw)
{
  tb->zoom = sb->zoom;
  tgw->digits.number = tb->zoom;
}
void clone_pattern(property_bank *tb, gui_widget *tgw, property_bank *sb, gui_widget *sgw)
{
  tb->pattern = sb->pattern;
  tgw->digits.number = tb->pattern;
}
