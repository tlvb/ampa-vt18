#include "graphics_base.h"
#include "gui.h"
#include "light.h"
#include "util.h"
#include "bank.h"
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
  property_bank banks[banks_n];
  const size_t settings_per_bank = 12;
  const size_t widgets_n = settings_per_bank*banks_n;
  gui_widget bank_widgets[widgets_n];




  for (size_t i=0; i<banks_n; ++i) {
    initialize_bank(&banks[i]);
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
    initialize_gui_widget(&gm, &widget[BANK_STROBEONOFF ], bank_x+420, bank_y,  40, 40, BUTTON                       ); // strobe-on
    initialize_gui_widget(&gm, &widget[BANK_STROBEAMOUNT], bank_x+470, bank_y,  40, 40, SWATCH2                      ); // strobe-amount
    initialize_gui_widget(&gm, &widget[BANK_UVAMOUNT    ], bank_x+530, bank_y,  40, 40, i >= 8 ? SWATCH2   : INACTIVE); // uv-amount
    initialize_gui_widget(&gm, &widget[BANK_ZOOM        ], bank_x+590, bank_y,  40, 40, i >= 8 ? TWODIGITS : INACTIVE); // zoom-amount
    initialize_gui_widget(&gm, &widget[BANK_PATTERN     ], bank_x+650, bank_y,  40, 40, TWODIGITS                    ); // program index

    update_bank_bpm(&banks[i],          &widget[BANK_BPM],          0);
    update_bank_maskn(&banks[i],        &widget[BANK_MASKN],        0);
    update_bank_hsv(&banks[i],          &widget[BANK_INHSV],        BANK_INHSV,   0, 0);
    update_bank_hsv(&banks[i],          &widget[BANK_HOLDHSV],      BANK_HOLDHSV, 0, 0);
    update_bank_hsv(&banks[i],          &widget[BANK_OUTHSV],       BANK_OUTHSV,  0, 0);
    update_bank_strobeamount(&banks[i], &widget[BANK_STROBEAMOUNT], 0);
    update_bank_uvamount(&banks[i],     &widget[BANK_UVAMOUNT],     0);
    update_bank_zoom(&banks[i],         &widget[BANK_ZOOM],         0);
    update_bank_pattern(&banks[i],      &widget[BANK_PATTERN],      0);
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

  program_data programs[banks_n];
  for (size_t i=0; i<8; ++i) {
    programs[i].tacc =    0;
    programs[i].tmax = 1000;
    programs[i].n = 8;
    programs[i].mask = calloc(8, sizeof(mask_state));
    programs[i].outputs = &fixtures[0];
    programs[i].mirrors = calloc(8, sizeof(fixture));
    programs[i].bank = &banks[i];
  }
  for (size_t i=8; i<12; ++i) {
    programs[i].tacc =    0;
    programs[i].tmax = 1000;
    programs[i].n = 6;
    programs[i].mask = calloc(6, sizeof(mask_state));
    programs[i].outputs = &fixtures[8];
    programs[i].mirrors = calloc(6, sizeof(fixture));
    programs[i].bank = &banks[i];
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

    for (size_t i=0; i<fixtures_n; ++i) {
      zero_values(&fixtures[i]);
    }
    for (size_t i=0; i<banks_n; ++i) {
      update_program(&programs[i], dt);
    }
    for (size_t i=0; i<fixtures_n; ++i) {
      saturate_values(&fixtures[i]);
    }
    update_gui_widgets_graphics(bank_widgets, settings_per_bank*banks_n, heart);
    update_fixture_widgets(light_widgets, fixtures_n);

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
        else if (e.key.keysym.sym == SDLK_BACKSPACE) {
          for (size_t i=0; i<widgets_n; ++i) {
            bank_widgets[i].select = false;
          }
        }
        else {
          const SDL_Keycode keys[12] = {SDLK_F1,
                                        SDLK_F2,
                                        SDLK_F3,
                                        SDLK_F4,
                                        SDLK_F5,
                                        SDLK_F6,
                                        SDLK_F7,
                                        SDLK_F8,
                                        SDLK_F9,
                                        SDLK_F10,
                                        SDLK_F11,
                                        SDLK_F12};
          const SDL_Keycode keys2[12] = {SDLK_1,
                                         SDLK_2,
                                         SDLK_3,
                                         SDLK_4,
                                         SDLK_5,
                                         SDLK_6,
                                         SDLK_7,
                                         SDLK_8,
                                         SDLK_9,
                                         SDLK_0,
                                         SDLK_LEFTBRACKET,
                                         SDLK_RIGHTBRACKET};
          for (size_t i=0; i<12; ++i) {
            if (e.key.keysym.sym == keys[i]) {
              size_t j = i*settings_per_bank + BANK_ONOFF;
              bank_widgets[j].button.on = !bank_widgets[j].button.on;
              bank_widgets[j].button.pressed = true;
              update_bank_onoff(&banks[i], &bank_widgets[j]);
              goto foundkey;
            }
          }
          for (size_t i=0; i<12; ++i) {
            if (e.key.keysym.sym == keys2[i]) {
              size_t j = i*settings_per_bank + BANK_WHITEONOFF;
              size_t k = i*settings_per_bank + BANK_INHSV;
              size_t l = i*settings_per_bank + BANK_HOLDHSV;
              size_t m = i*settings_per_bank + BANK_OUTHSV;
              bank_widgets[j].button.on = !bank_widgets[j].button.on;
              bank_widgets[j].button.pressed = true;
              update_bank_white(&banks[i], &bank_widgets[j]);
              update_bank_hsv(&banks[i], &bank_widgets[k], BANK_INHSV,   0, 0);
              update_bank_hsv(&banks[i], &bank_widgets[l], BANK_HOLDHSV, 0, 0);
              update_bank_hsv(&banks[i], &bank_widgets[m], BANK_OUTHSV,  0, 0);
              goto foundkey;
            }
          }
        foundkey:
          while (false);
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
        for (size_t i=0; i<banks_n; ++i) {
          property_bank *b = &banks[i];
          gui_widget *widget = &bank_widgets[i*settings_per_bank];
          for (size_t j=0; j<settings_per_bank; ++j) {
            size_t k = i*settings_per_bank + j;
            if (is_inside(&widget[j].dims, mx, my)) {
              widget[j].hover = true;
              if (widget[j].type == BUTTON && e.button.button == SDL_BUTTON_LEFT) {
                widget[j].button.on = !widget[j].button.on;
                widget[j].button.pressed = true;
              }
              else if (e.button.button == SDL_BUTTON_RIGHT) {
                widget[j].select = !widget[j].select;
              }
              switch (j) {
              case BANK_ONOFF:
                update_bank_onoff(b, &widget[j]);
                break;
              case BANK_WHITEONOFF:
                update_bank_white(b, &widget[j]);
                update_bank_hsv(b, &widget[BANK_INHSV],   BANK_INHSV,   0, 0);
                update_bank_hsv(b, &widget[BANK_HOLDHSV], BANK_HOLDHSV, 0, 0);
                update_bank_hsv(b, &widget[BANK_OUTHSV],  BANK_OUTHSV,  0, 0);
                break;
              case BANK_STROBEONOFF:
                update_bank_white(b, &widget[j]);
                break;
              }
            }
            else {
              bank_widgets[i].hover = false;
            }
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
                update_bank_bpm(&banks[i], &bank_widgets[k], e.wheel.y * (shift ? 10 : 1));
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
        for (size_t i=0; i<banks_n; ++i) {
          size_t j = i*settings_per_bank + BANK_ONOFF;
          size_t k = i*settings_per_bank + BANK_WHITEONOFF;
          size_t l = i*settings_per_bank + BANK_STROBEONOFF;

          bank_widgets[j].button.pressed = false;

          bank_widgets[k].button.pressed = false;

          bank_widgets[l].button.pressed = false;

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
    previous_iteration_time = current_iteration_time;
  }

}
