#include "graphics_base.h"
#include "gui.h"
#include "light.h"
#include "util.h"
typedef uint_fast8_t bank_index;
#define BANK_BPM           0
#define BANK_ONOFF         1
#define BANK_MASKN         2
#define BANK_INHSV         3
#define BANK_HOLDHSV       4
#define BANK_OUTHSV        5
#define BANK_WHITEONOFF    6
#define BANK_STROBEONOFF   7
#define BANK_STROBEAMOUNT  8
#define BANK_UVAMOUNT      9
#define BANK_ZOOM         10
#define BANK_PATTERN      11
typedef uint_fast8_t rangespec;
#define BANK_HUEAVG    0
#define BANK_HUEWINDOW 1
#define BANK_VALMAX    2
#define BANK_VALWINDOW 3
void update_bank_maskn(bank *b, gui_widget* gw, int16_t delta);
void update_bank_hsv(bank *b, gui_widget* gw, bank_index id, int16_t delta, rangespec rs);
void update_bank_strobeamount(bank *b, gui_widget* gw, int16_t delta);
void update_bank_uvamount(bank *b, gui_widget* gw, int16_t delta);
void update_bank_zoom(bank *b, gui_widget* gw, int16_t delta);
void update_bank_pattern(bank *b, gui_widget* gw, int16_t delta);
void update_bank_maskn(bank *b, gui_widget* gw, int16_t delta)
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
void update_bank_hsv(bank *b, gui_widget* gw, bank_index id, int16_t delta, rangespec rs)
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
  hsv2rgb(&gw->swatch.red[0], &gw->swatch.green[0], &gw->swatch.blue[0], r->hueavg, 1.0f, r->valmax);
  hsv2rgb(&gw->swatch.red[1], &gw->swatch.green[1], &gw->swatch.blue[1], fmodf(r->hueavg+r->huewindow+3600.0f, 360.0f), 1.0f, 0.5f+r->valmax/2.0f);
  hsv2rgb(&gw->swatch.red[2], &gw->swatch.green[2], &gw->swatch.blue[2], r->hueavg, 1.0f, r->valmax*r->valwindow);
  hsv2rgb(&gw->swatch.red[3], &gw->swatch.green[3], &gw->swatch.blue[3], fmodf(r->hueavg-r->huewindow+3600.0f, 360.0f),1.0f, 0.5f+r->valmax/2.0f);
}
void update_bank_strobeamount(bank *b, gui_widget* gw, int16_t delta)
{
  b->strobe_amount = fmaxf(fminf(b->strobe_amount+delta/20.0f, 1.0f), 0.0f);
  gw->swatch.red[0] = gw->swatch.green[0] = gw->swatch.blue[0] = 0.5f+b->strobe_amount/2.0f;
  gw->swatch.red[1] = gw->swatch.green[1] = gw->swatch.blue[1] = 0.5f-b->strobe_amount/2.0f;
}
void update_bank_uvamount(bank *b, gui_widget* gw, int16_t delta)
{
  b->uv_amount = fmaxf(fminf(b->uv_amount+delta/10.0f, 1.0f), 0.0f);
  gw->swatch.red[0] = b->uv_amount*0.4;
  gw->swatch.green[0] = 0.0f;
  gw->swatch.blue[0] = b->uv_amount;
  gw->swatch.red[1] = b->uv_amount*0.2;
  gw->swatch.green[1] = 0.0f;
  gw->swatch.blue[1] = b->uv_amount*0.5;
}
void update_bank_zoom(bank *b, gui_widget* gw, int16_t delta)
{
  
}
void update_bank_pattern(bank *b, gui_widget* gw, int16_t delta)
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
int main(int argc, char **argv)
{

  // DMX SETUP
  /* conduit con; */
  /* connect_conduit(&con); */

  // GRAPHICS SETUP
  windowdata wd = WINDOW_DATA_DEFAULT;
  if (argc == 2 && strcmp("-w", argv[1]) == 0) {
    wd.width = 1280;
    wd.height = 800;
  }
  else {
    wd.width = 1280;
    wd.height = 800;
    wd.flags = SDL_WINDOW_FULLSCREEN;
  }
  if (!create_gl_window(&wd)) {
    fprintf(stderr, "failed creating a gl window\n");
    return 1;
  }
  shaderdata sd;
  if (!create_gl_shader(&sd, "shadersrc/vertex.glsl", "shadersrc/fragment.glsl")) {
    fprintf(stderr, "failed creating a gl shader\n");
    destroy_gl_window(&wd);
    return 1;
  }
  glUseProgram(sd.program);
  GLenum e = glGetError();
  if (e != GL_NO_ERROR) {
    fprintf(stderr, "error using shader program\n");
    rm_shader_program(&sd);
    destroy_gl_window(&wd);
    return 1;
  }
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  GLint position_ref = glGetAttribLocation(sd.program, "position");
  GLint position_buf_ref;

  GLint color_ref = glGetAttribLocation(sd.program, "color");
  GLint color_buf_ref;

  glGenBuffers(1, &position_buf_ref);
  glGenBuffers(1, &color_buf_ref);

  const size_t nvert = 8000;
  model gm = {0};
  alloc_model(&gm, nvert);

  const size_t banks_n = 12;
  bank banks[banks_n];

  const size_t settings_per_bank = 12;
  const size_t widgets_n = settings_per_bank*banks_n;
  gui_widget bank_widgets[widgets_n];

  for (size_t i=0; i<banks_n; ++i) {
    memset(&banks[i], 0, sizeof(bank));
    screen_dim bank_x = 450;
    screen_dim bank_y = 40+50*i + (i>=4?60:0) + (i>=8?60:0);
    gui_widget *widget = &bank_widgets[i*settings_per_bank];
    initialize_gui_widget(&gm, &widget[BANK_BPM         ], bank_x+  0, bank_y,  80, 40, FOURDIGITS                   ); // bpm
    initialize_gui_widget(&gm, &widget[BANK_ONOFF       ], bank_x+ 90, bank_y,  40, 40, BUTTON                       ); // active
    initialize_gui_widget(&gm, &widget[BANK_MASKN       ], bank_x+140, bank_y,  40, 40, TWODIGITS                    ); // mask count change type
    initialize_gui_widget(&gm, &widget[BANK_INHSV       ], bank_x+200, bank_y,  40, 40, SWATCH4                      ); // in
    initialize_gui_widget(&gm, &widget[BANK_HOLDHSV     ], bank_x+250, bank_y,  40, 40, SWATCH4                      ); // hold
    initialize_gui_widget(&gm, &widget[BANK_OUTHSV      ], bank_x+300, bank_y,  40, 40, SWATCH4                      ); // out
    initialize_gui_widget(&gm, &widget[BANK_WHITEONOFF  ], bank_x+360, bank_y,  40, 40, BUTTON                       ); // white-on
    initialize_gui_widget(&gm, &widget[BANK_STROBEONOFF ], bank_x+470, bank_y,  40, 40, BUTTON                       ); // strobe-on
    initialize_gui_widget(&gm, &widget[BANK_STROBEAMOUNT], bank_x+520, bank_y,  40, 40, SWATCH2                      ); // strobe-amount
    initialize_gui_widget(&gm, &widget[BANK_UVAMOUNT    ], bank_x+580, bank_y,  40, 40, i >= 8 ? SWATCH2   : INACTIVE); // uv-amount
    initialize_gui_widget(&gm, &widget[BANK_ZOOM        ], bank_x+640, bank_y,  40, 40, i >= 8 ? TWODIGITS : INACTIVE); // zoom-amount
    initialize_gui_widget(&gm, &widget[BANK_PATTERN     ], bank_x+740, bank_y,  40, 40, TWODIGITS                    ); // program index
  }

  const size_t fixtures_n = 8 + 4 + 2;

  fixture fixtures[fixtures_n];
  for (size_t i=0; i<fixtures_n; ++i) {
    if (i < 8) {
      fixtures[i].type = LEDWASH;
      fixtures[i].base_channel = 0;
    }
    else if (i < 8+4) {
      fixtures[i].type = LEDMH;
      fixtures[i].base_channel = 0;
    }
    else {
      fixtures[i].type = LEDMH2;
      fixtures[i].base_channel = 0;
    }
  }

  fixture_widget light_widgets[fixtures_n];

  for (size_t i=0; i<fixtures_n; ++i) {
    initialize_fixture_widget(&gm, &light_widgets[i], &fixtures[i], 40+(i&3)*90, 40+(i>>2)*90, 80, 80);
  }

  uint32_t previous_iteration_time = SDL_GetTicks();
  bool main_loop = true;
  bool shift = false;
  bool ctrl = false;
  bool alt = false;
  while (main_loop) {
    uint32_t current_iteration_time = SDL_GetTicks();
    uint32_t dt = current_iteration_time - previous_iteration_time;
    bool heart = (current_iteration_time & 0x00000200) != 0;


    // GL STUFF
    glBindBuffer(GL_ARRAY_BUFFER, position_buf_ref);
    glBufferData(GL_ARRAY_BUFFER, gm.vtx_n*DIM_PER_VTX*sizeof(GLfloat), gm.points, GL_STATIC_DRAW);
    glVertexAttribPointer(position_ref, DIM_PER_VTX, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, NULL);
    glEnableVertexAttribArray(position_ref);
    glBindBuffer(GL_ARRAY_BUFFER, color_buf_ref);
    glBufferData(GL_ARRAY_BUFFER, gm.vtx_n*CLR_PER_VTX*sizeof(GLfloat), gm.colors, GL_STATIC_DRAW);
    glVertexAttribPointer(color_ref, CLR_PER_VTX, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, NULL);
    glEnableVertexAttribArray(color_ref);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, gm.vtx_n);
    SDL_GL_SwapWindow(wd.window);

    update_gui_widgets_graphics(bank_widgets, settings_per_bank*banks_n, heart);
    update_fixture_widgets(light_widgets, fixtures_n);

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_q) {
          main_loop = false;
        }
        else if (e.key.keysym.sym == SDLK_LSHIFT) {
          shift = true;
        }
        else if (e.key.keysym.sym == SDLK_LCTRL) {
          ctrl = true;
        }
        else if (e.key.keysym.sym == SDLK_LALT) {
          alt = true;
        }
      }
      else if (e.type == SDL_MOUSEMOTION) {
        const screen_dim mx = e.motion.x;
        const screen_dim my = e.motion.y;
        for (size_t i=0; i<widgets_n; ++i) {
          if (is_inside(&bank_widgets[i].dims, mx, my)) {
            bank_widgets[i].hover = true;
          }
          else {
            bank_widgets[i].hover = false;
          }
        }
      }
      else if (e.type == SDL_MOUSEBUTTONDOWN) {
        const screen_dim mx = e.motion.x;
        const screen_dim my = e.motion.y;
        for (size_t i=0; i<widgets_n; ++i) {
          if (is_inside(&bank_widgets[i].dims, mx, my)) {
            bank_widgets[i].hover = true;
            if (bank_widgets[i].type == BUTTON && e.button.button == SDL_BUTTON_LEFT) {
              bank_widgets[i].button.on = !bank_widgets[i].button.on;
              bank_widgets[i].button.pressed = true;
            }
            else if (e.button.button == SDL_BUTTON_RIGHT) {
              bank_widgets[i].select = !bank_widgets[i].select;
            }
          }
          else {
            bank_widgets[i].hover = false;
          }
        }
      }
      else if (e.type == SDL_MOUSEWHEEL) {
        rangespec rs = BANK_HUEAVG;
        if (shift) {
          rs = BANK_VALMAX;
        }
        else if (ctrl) {
          rs = BANK_VALWINDOW;
        }
        else if (alt) {
          rs = BANK_HUEWINDOW;
        }
        for (size_t i=0; i<banks_n; ++i) {
          for (size_t j=0; j<settings_per_bank; ++j) {
            size_t k = i*settings_per_bank + j;
            if (bank_widgets[k].select) {
              switch (j) {
              case BANK_BPM:
                break;
              case BANK_ONOFF:
                break;
              case BANK_MASKN:
                update_bank_maskn(&banks[i], &bank_widgets[k], e.wheel.y);
                break;
              case BANK_INHSV:
                update_bank_hsv(&banks[i], &bank_widgets[k], BANK_INHSV, e.wheel.y, rs);
                break;
              case BANK_HOLDHSV:
                update_bank_hsv(&banks[i], &bank_widgets[k], BANK_HOLDHSV, e.wheel.y, rs);
                break;
              case BANK_OUTHSV:
                update_bank_hsv(&banks[i], &bank_widgets[k], BANK_OUTHSV, e.wheel.y, rs);
                break;
              case BANK_WHITEONOFF:
                break;
              case BANK_STROBEONOFF:
                break;
              case BANK_STROBEAMOUNT:
                update_bank_strobeamount(&banks[i], &bank_widgets[k], e.wheel.y);
                break;
              case BANK_UVAMOUNT:
                update_bank_uvamount(&banks[i], &bank_widgets[k], e.wheel.y);
                break;
              case BANK_ZOOM:
                update_bank_zoom(&banks[i], &bank_widgets[k], e.wheel.y);
                break;
              case BANK_PATTERN:
                update_bank_pattern(&banks[i], &bank_widgets[k], e.wheel.y);
                break;
              }
            }
          }
        }
      }
      else if (e.type == SDL_KEYUP) {
        if (e.key.keysym.sym == SDLK_LSHIFT) {
          shift = false;
        }
        else if (e.key.keysym.sym == SDLK_LCTRL) {
          ctrl = false;
        }
        else if (e.key.keysym.sym == SDLK_LALT) {
          alt = false;
        }
      }
      else if (e.type == SDL_MOUSEBUTTONUP) {
        for (size_t i=0; i<widgets_n; ++i) {
          if (bank_widgets[i].type == BUTTON) {
            bank_widgets[i].button.pressed = false;
          }
        }
      }
    }
  }

}
