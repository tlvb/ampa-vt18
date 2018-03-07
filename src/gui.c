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
  case TWODIGITS:
    return TRI_PER_TWODIGIT;
  case FOURDIGITS:
    return TRI_PER_FOURDIGIT;
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
static void initialize_digit(GLfloat *points, GLfloat *colors,  digit *d, float x, float y, float s)
{
	d->pos[0] = x;
	d->pos[1] = y;
	d->s = s;

	d->points = points;
	d->colors = colors;

	const GLfloat pts[TRI_PER_7SEG*VTX_PER_TRI*DIM_PER_VTX] = {
		x+4*s, y+1*s,  x+5*s, y+1*s,  x+5*s, y+3*s, // a
		x+4*s, y+1*s,  x+5*s, y+3*s,  x+4*s, y+3*s, // a

		x+4*s, y+4*s,  x+5*s, y+4*s,  x+5*s, y+6*s, // b
		x+4*s, y+4*s,  x+5*s, y+6*s,  x+4*s, y+6*s, // b

		x+1*s, y+6*s,  x+4*s, y+6*s,  x+4*s, y+7*s, // c
		x+1*s, y+6*s,  x+4*s, y+7*s,  x+1*s, y+7*s, // c

		x+0*s, y+4*s,  x+1*s, y+4*s,  x+1*s, y+6*s, // d
		x+0*s, y+4*s,  x+1*s, y+6*s,  x+0*s, y+6*s, // d

		x+0*s, y+1*s,  x+1*s, y+1*s,  x+1*s, y+3*s, // e
		x+0*s, y+1*s,  x+1*s, y+3*s,  x+0*s, y+3*s, // e

		x+1*s, y+0*s,  x+4*s, y+0*s,  x+4*s, y+1*s, // f
		x+1*s, y+0*s,  x+4*s, y+1*s,  x+1*s, y+1*s, // f

		x+1*s, y+3*s,  x+4*s, y+3*s,  x+4*s, y+4*s, // g
		x+1*s, y+3*s,  x+4*s, y+4*s,  x+1*s, y+4*s  // g
	};
	memcpy(d->points, pts, sizeof(GLfloat)*TRI_PER_7SEG*VTX_PER_TRI*DIM_PER_VTX);
	memset(d->colors, 0,   sizeof(GLfloat)*TRI_PER_7SEG*VTX_PER_TRI*CLR_PER_VTX);

  d->off_color[0] = 0.1;
  d->off_color[1] = 0;
  d->off_color[2] = 0;
  d->on_color[0] = 1;
  d->on_color[1] = 0;
  d->on_color[2] = 0;
}
static void initialize_twodigits(gui_widget *digits)
{
  const screen_dim x = digits->dims.pos[0];
  const screen_dim y = digits->dims.pos[1];
  const screen_dim w = digits->dims.size[0];
  const screen_dim h = digits->dims.size[1];
  const screen_dim m = 3;
  for (size_t i=0; i<TRI_PER_TWODIGIT*VTX_PER_TRI; ++i) {
    digits->m.points[i*DIM_PER_VTX+0] = rand()*100;
    digits->m.points[i*DIM_PER_VTX+1] = rand()*100;
  }
  for (size_t i=0; i<TRI_PER_TWODIGIT*VTX_PER_TRI; ++i) {
    digits->m.colors[i*CLR_PER_VTX+0] = 0.15f;
    digits->m.colors[i*CLR_PER_VTX+1] = 0.15f;
    digits->m.colors[i*CLR_PER_VTX+2] = 0.15f;
  }
  digits->m.points[ 0] = x;
  digits->m.points[ 1] = y;
  digits->m.points[ 2] = x+w;
  digits->m.points[ 3] = y;
  digits->m.points[ 4] = x+w;
  digits->m.points[ 5] = y+h;

  digits->m.points[ 6] = x;
  digits->m.points[ 7] = y;
  digits->m.points[ 8] = x+w;
  digits->m.points[ 9] = y+h;
  digits->m.points[10] = x;
  digits->m.points[11] = y+h;

  digits->m.points[12] = x+m;
  digits->m.points[13] = y+m;
  digits->m.points[14] = x+w-m;
  digits->m.points[15] = y+m;
  digits->m.points[16] = x+w-m;
  digits->m.points[17] = y+h-m;

  digits->m.points[18] = x+m;
  digits->m.points[19] = y+m;
  digits->m.points[20] = x+w-m;
  digits->m.points[21] = y+h-m;
  digits->m.points[22] = x+m;
  digits->m.points[23] = y+h-m;

  screen_dim s = (w-4*m)/14.0f <= (h-4*m)/9.0f ? (w-4*m)/14 : (h-4*m)/9;

  initialize_digit(digits->m.points+4*VTX_PER_TRI*DIM_PER_VTX, digits->m.colors+4*VTX_PER_TRI*CLR_PER_VTX, &digits->digits.digits[0], x+w/2-6*s, y+h/2-s*9/2, s);
  initialize_digit(digits->m.points+(4+TRI_PER_7SEG)*VTX_PER_TRI*DIM_PER_VTX, digits->m.colors+(4+TRI_PER_7SEG)*VTX_PER_TRI*CLR_PER_VTX, &digits->digits.digits[1], x+w/2+s, y+h/2-s*9/2, s);

	for (size_t i=0; i<4; ++i) {
	}
}
static void initialize_fourdigits(gui_widget *digits)
{
  const screen_dim x = digits->dims.pos[0];
  const screen_dim y = digits->dims.pos[1];
  const screen_dim w = digits->dims.size[0];
  const screen_dim h = digits->dims.size[1];
  const screen_dim m = 3;
  for (size_t i=0; i<TRI_PER_TWODIGIT*VTX_PER_TRI; ++i) {
    digits->m.points[i*DIM_PER_VTX+0] = rand()*100;
    digits->m.points[i*DIM_PER_VTX+1] = rand()*100;
  }
  for (size_t i=0; i<TRI_PER_TWODIGIT*VTX_PER_TRI; ++i) {
    digits->m.colors[i*CLR_PER_VTX+0] = 0.15f;
    digits->m.colors[i*CLR_PER_VTX+1] = 0.15f;
    digits->m.colors[i*CLR_PER_VTX+2] = 0.15f;
  }
  digits->m.points[ 0] = x;
  digits->m.points[ 1] = y;
  digits->m.points[ 2] = x+w;
  digits->m.points[ 3] = y;
  digits->m.points[ 4] = x+w;
  digits->m.points[ 5] = y+h;

  digits->m.points[ 6] = x;
  digits->m.points[ 7] = y;
  digits->m.points[ 8] = x+w;
  digits->m.points[ 9] = y+h;
  digits->m.points[10] = x;
  digits->m.points[11] = y+h;

  digits->m.points[12] = x+m;
  digits->m.points[13] = y+m;
  digits->m.points[14] = x+w-m;
  digits->m.points[15] = y+m;
  digits->m.points[16] = x+w-m;
  digits->m.points[17] = y+h-m;

  digits->m.points[18] = x+m;
  digits->m.points[19] = y+m;
  digits->m.points[20] = x+w-m;
  digits->m.points[21] = y+h-m;
  digits->m.points[22] = x+m;
  digits->m.points[23] = y+h-m;

  screen_dim s = (w-4*m)/24.0f <= (h-4*m)/9.0f ? (w-4*m)/24 : (h-4*m)/9;

  initialize_digit(digits->m.points+4*VTX_PER_TRI*DIM_PER_VTX, digits->m.colors+4*VTX_PER_TRI*CLR_PER_VTX, &digits->digits.digits[0], x+w/2-13*s, y+h/2-s*9/2, s);
  initialize_digit(digits->m.points+(4+TRI_PER_7SEG)*VTX_PER_TRI*DIM_PER_VTX, digits->m.colors+(4+TRI_PER_7SEG)*VTX_PER_TRI*CLR_PER_VTX, &digits->digits.digits[1], x+w/2-6*s, y+h/2-s*9/2, s);
  initialize_digit(digits->m.points+(4+2*TRI_PER_7SEG)*VTX_PER_TRI*DIM_PER_VTX, digits->m.colors+(4+2*TRI_PER_7SEG)*VTX_PER_TRI*CLR_PER_VTX, &digits->digits.digits[2], x+w/2+s, y+h/2-s*9/2, s);
  initialize_digit(digits->m.points+(4+3*TRI_PER_7SEG)*VTX_PER_TRI*DIM_PER_VTX, digits->m.colors+(4+3*TRI_PER_7SEG)*VTX_PER_TRI*CLR_PER_VTX, &digits->digits.digits[3], x+w/2+8*s, y+h/2-s*9/2, s);

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
  case TWODIGITS:
    initialize_twodigits(gw);
    break;
  case FOURDIGITS:
    initialize_fourdigits(gw);
    break;
  default:
    fprintf(stderr, "no such gui widget type: %" PRI_GWT "\n", type);
    exit(1);
  }
}


static inline void update_digit_graphics(digit *d, int8_t v)
{
	// A 0
	if (v==0 || v==1 || v==2 || v==3 || v==4 ||                 v==7 || v==8 || v==9) {
		memcpy(d->colors+ 0, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+ 3, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+ 6, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+ 9, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+12, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+15, d->on_color, sizeof(GLfloat)*3);
	}
	else {
		memcpy(d->colors+ 0, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+ 3, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+ 6, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+ 9, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+12, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+15, d->off_color, sizeof(GLfloat)*3);
	}
	// B 1
	if (v==0 || v==1 ||         v==3 || v==4 || v==5 || v==6 || v==7 || v==8 || v==9) {
		memcpy(d->colors+18, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+21, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+24, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+27, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+30, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+33, d->on_color, sizeof(GLfloat)*3);
	}
	else {
		memcpy(d->colors+18, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+21, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+24, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+27, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+30, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+33, d->off_color, sizeof(GLfloat)*3);
	}
	// C 2
	if (v==0 ||         v==2 || v==3 ||         v==5 || v==6 ||         v==8 || v==9 || v<0) {
		memcpy(d->colors+36, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+39, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+42, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+45, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+48, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+51, d->on_color, sizeof(GLfloat)*3);
	}
	else {
		memcpy(d->colors+36, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+39, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+42, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+45, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+48, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+51, d->off_color, sizeof(GLfloat)*3);
	}
	// D 3
	if (v==0 ||         v==2 ||                         v==6 ||         v==8        ) {
		memcpy(d->colors+54, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+57, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+60, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+63, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+66, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+69, d->on_color, sizeof(GLfloat)*3);
	}
	else {
		memcpy(d->colors+54, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+57, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+60, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+63, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+66, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+69, d->off_color, sizeof(GLfloat)*3);
	}
	// E 4
	if (v==0 ||                         v==4 || v==5 || v==6 || v==7 || v==8 || v==9) {
		memcpy(d->colors+72, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+75, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+78, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+81, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+84, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+87, d->on_color, sizeof(GLfloat)*3);
	}
	else {
		memcpy(d->colors+72, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+75, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+78, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+81, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+84, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+87, d->off_color, sizeof(GLfloat)*3);
	}
	// F 5
	if (v==0 ||         v==2 || v==3 ||         v==5 || v==6 || v==7 || v==8 || v==9 || v > 9) {
		memcpy(d->colors+ 90, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+ 93, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+ 96, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+ 99, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+102, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+105, d->on_color, sizeof(GLfloat)*3);
	}
	else {
		memcpy(d->colors+ 90, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+ 93, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+ 96, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+ 99, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+102, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+105, d->off_color, sizeof(GLfloat)*3);
	}
	// G 6
	if (                v==2 || v==3 || v==4 || v==5 || v==6 ||         v==8 || v==9) {
		memcpy(d->colors+108, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+111, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+114, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+117, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+120, d->on_color, sizeof(GLfloat)*3);
		memcpy(d->colors+123, d->on_color, sizeof(GLfloat)*3);
	}
	else {
		memcpy(d->colors+108, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+111, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+114, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+117, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+120, d->off_color, sizeof(GLfloat)*3);
		memcpy(d->colors+123, d->off_color, sizeof(GLfloat)*3);
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
static inline void update_twodigit_graphics(gui_widget *digits, bool heart)
{
  float r, g, b;
  if (digits->hover) {
    r = g = b = 1.0f;
  }
  else if (digits->select) {
    r = g = heart ? 1.0f : 0.5f;
    b = 0.0f;
  }
  else {
    r = g = b = 0.0f;
  }

  digits->m.colors[ 0] = digits->m.colors[ 3] = digits->m.colors[ 6] = r;
  digits->m.colors[ 1] = digits->m.colors[ 4] = digits->m.colors[ 7] = g;
  digits->m.colors[ 2] = digits->m.colors[ 5] = digits->m.colors[ 8] = b;
  digits->m.colors[ 9] = digits->m.colors[12] = digits->m.colors[15] = r;
  digits->m.colors[10] = digits->m.colors[13] = digits->m.colors[16] = g;
  digits->m.colors[11] = digits->m.colors[14] = digits->m.colors[17] = b;

  digits->m.colors[18] = digits->m.colors[21] = digits->m.colors[24] = 0.0f;
  digits->m.colors[19] = digits->m.colors[22] = digits->m.colors[25] = 0.0f;
  digits->m.colors[20] = digits->m.colors[23] = digits->m.colors[26] = 0.0f;
  digits->m.colors[27] = digits->m.colors[30] = digits->m.colors[33] = 0.0f;
  digits->m.colors[28] = digits->m.colors[31] = digits->m.colors[34] = 0.0f;
  digits->m.colors[29] = digits->m.colors[32] = digits->m.colors[35] = 0.0f;

  update_digit_graphics(&digits->digits.digits[0], 8);
  update_digit_graphics(&digits->digits.digits[1], 8);
}
static inline void update_fourdigit_graphics(gui_widget *digits, bool heart)
{
  float r, g, b;
  if (digits->hover) {
    r = g = b = 1.0f;
  }
  else if (digits->select) {
    r = g = heart ? 1.0f : 0.5f;
    b = 0.0f;
  }
  else {
    r = g = b = 0.0f;
  }

  digits->m.colors[ 0] = digits->m.colors[ 3] = digits->m.colors[ 6] = r;
  digits->m.colors[ 1] = digits->m.colors[ 4] = digits->m.colors[ 7] = g;
  digits->m.colors[ 2] = digits->m.colors[ 5] = digits->m.colors[ 8] = b;
  digits->m.colors[ 9] = digits->m.colors[12] = digits->m.colors[15] = r;
  digits->m.colors[10] = digits->m.colors[13] = digits->m.colors[16] = g;
  digits->m.colors[11] = digits->m.colors[14] = digits->m.colors[17] = b;

  digits->m.colors[18] = digits->m.colors[21] = digits->m.colors[24] = 0.0f;
  digits->m.colors[19] = digits->m.colors[22] = digits->m.colors[25] = 0.0f;
  digits->m.colors[20] = digits->m.colors[23] = digits->m.colors[26] = 0.0f;
  digits->m.colors[27] = digits->m.colors[30] = digits->m.colors[33] = 0.0f;
  digits->m.colors[28] = digits->m.colors[31] = digits->m.colors[34] = 0.0f;
  digits->m.colors[29] = digits->m.colors[32] = digits->m.colors[35] = 0.0f;

  update_digit_graphics(&digits->digits.digits[0], 8);
  update_digit_graphics(&digits->digits.digits[1], 8);
  update_digit_graphics(&digits->digits.digits[2], 8);
  update_digit_graphics(&digits->digits.digits[3], 8);
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
    case TWODIGITS:
      update_twodigit_graphics(&gw[i], heart);
      break;
    case FOURDIGITS:
      update_fourdigit_graphics(&gw[i], heart);
      break;
    default:
      fprintf(stderr, "no such gui widget type: %" PRI_GWT "\n", gw->type);
      exit(1);
    }
    // }
  }
}
