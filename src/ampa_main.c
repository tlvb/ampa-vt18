#include "graphics_base.h"
#include "gui.h"
#include "light.h"
#include "util.h"
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
  const size_t settings_per_bank = 13;
  const size_t widgets_n = settings_per_bank*banks_n;
  gui_widget bank_widgets[widgets_n];

  for (size_t i=0; i<banks_n; ++i) {
    screen_dim bank_x = 400;
    screen_dim bank_y = 40+50*i + (i>=4?60:0) + (i>=8?60:0);
    gui_widget *widget = &bank_widgets[i*settings_per_bank];
    initialize_gui_widget(&gm, &widget[ 0], bank_x+  0, bank_y,  80, 40, FOURDIGITS                   ); // bpm
    initialize_gui_widget(&gm, &widget[ 1], bank_x+ 90, bank_y,  40, 40, BUTTON                       ); // active
    initialize_gui_widget(&gm, &widget[ 2], bank_x+140, bank_y,  40, 40, TWODIGITS                    ); // mask count change type
    initialize_gui_widget(&gm, &widget[ 3], bank_x+200, bank_y,  40, 40, SWATCH4                      ); // in
    initialize_gui_widget(&gm, &widget[ 4], bank_x+250, bank_y,  40, 40, SWATCH4                      ); // hold
    initialize_gui_widget(&gm, &widget[ 5], bank_x+300, bank_y,  40, 40, SWATCH4                      ); // out
    initialize_gui_widget(&gm, &widget[ 6], bank_x+360, bank_y,  40, 40, BUTTON                       ); // white-on
    initialize_gui_widget(&gm, &widget[ 7], bank_x+410, bank_y,  40, 40, SWATCH2                      ); // white-amount
    initialize_gui_widget(&gm, &widget[ 8], bank_x+470, bank_y,  40, 40, BUTTON                       ); // strobe-on
    initialize_gui_widget(&gm, &widget[ 9], bank_x+520, bank_y,  40, 40, SWATCH2                      ); // strobe-amount
    initialize_gui_widget(&gm, &widget[10], bank_x+580, bank_y,  40, 40, i >= 8 ? SWATCH2   : INACTIVE); // uv-amount
    initialize_gui_widget(&gm, &widget[11], bank_x+640, bank_y,  40, 40, i >= 8 ? TWODIGITS : INACTIVE); // zoom-amount
    initialize_gui_widget(&gm, &widget[12], bank_x+740, bank_y,  40, 40, TWODIGITS                    ); // program index
  }

  const size_t fixtures_n = 5;

  fixture fix[5] = {{ .type = SINGLECHANNEL, .base_channel =  20 },
                    { .type = LEDPAR56,      .base_channel =  40 },
                    { .type = LEDWASH,       .base_channel =  60 },
                    { .type = LEDMH,         .base_channel =  80 },
                    { .type = LEDMH2,        .base_channel = 100 }};
  fixture_widget simple_light[fixtures_n];

  for (size_t i=0; i<fixtures_n; ++i) {
    initialize_fixture_widget(&gm, &simple_light[i], &fix[i], 500, 100+60*i, 50, 50);
  }




  uint32_t previous_iteration_time = SDL_GetTicks();
  bool main_loop = true;
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
    update_fixture_widgets(simple_light, fixtures_n);

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_q) {
          main_loop = false;
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
      else if (e.type == SDL_KEYUP || e.type == SDL_MOUSEBUTTONUP) {
        for (size_t i=0; i<widgets_n; ++i) {
          bank_widgets[i].button.pressed = false;
        }
      }
    }
  }

}
