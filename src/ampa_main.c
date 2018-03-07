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

  gui_widget simple_widget[13] = {0};
  const size_t widgets_n = 13;

  initialize_gui_widget(&gm, &simple_widget[ 0], 100,     500,  30, 30, BUTTON    ); // active
  initialize_gui_widget(&gm, &simple_widget[ 1], 160+ 10, 500,  30, 30, TWODIGITS ); // mask count change type
  initialize_gui_widget(&gm, &simple_widget[ 2], 220+ 20, 500,  30, 30, SWATCH4   ); // in
  initialize_gui_widget(&gm, &simple_widget[ 3], 280+ 20, 500,  30, 30, SWATCH4   ); // hold
  initialize_gui_widget(&gm, &simple_widget[ 4], 340+ 20, 500,  30, 30, SWATCH4   ); // out
  initialize_gui_widget(&gm, &simple_widget[ 5], 400+ 30, 500,  30, 30, BUTTON    ); // white-on
  initialize_gui_widget(&gm, &simple_widget[ 6], 460+ 30, 500,  30, 30, SWATCH2   ); // white-amount
  initialize_gui_widget(&gm, &simple_widget[ 7], 520+ 40, 500,  30, 30, BUTTON    ); // strobe-on
  initialize_gui_widget(&gm, &simple_widget[ 8], 580+ 40, 500,  30, 30, SWATCH2   ); // strobe-amount
  initialize_gui_widget(&gm, &simple_widget[ 9], 640+ 50, 500,  30, 30, SWATCH2   ); // uv-amount
  initialize_gui_widget(&gm, &simple_widget[10], 700+ 60, 500,  60, 30, FOURDIGITS); // zoom-amount change type?
  initialize_gui_widget(&gm, &simple_widget[11], 760+120, 500,  30, 30, INACTIVE  ); // inactive
  initialize_gui_widget(&gm, &simple_widget[12], 820+120, 500,  30, 30, INACTIVE  ); // inactive

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

    update_gui_widgets_graphics(simple_widget, widgets_n, heart);
    update_fixture_widgets(simple_light, fixtures_n);

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_q) {
          main_loop = false;
        }
        else if (e.key.keysym.sym == SDLK_SPACE) {
          simple_widget[0].select = !simple_widget[0].select;
        }
        else if (e.key.keysym.sym == SDLK_RETURN) {
          simple_widget[0].button.on = !simple_widget[0].button.on;
        }
      }
      else if (e.type == SDL_MOUSEMOTION) {
        const screen_dim mx = e.motion.x;
        const screen_dim my = e.motion.y;
        for (size_t i=0; i<widgets_n; ++i) {
          if (is_inside(&simple_widget[i].dims, mx, my)) {
            simple_widget[i].hover = true;
          }
          else {
            simple_widget[i].hover = false;
          }
        }
      }
      else if (e.type == SDL_MOUSEBUTTONDOWN) {
        const screen_dim mx = e.motion.x;
        const screen_dim my = e.motion.y;
        for (size_t i=0; i<widgets_n; ++i) {
          if (is_inside(&simple_widget[i].dims, mx, my)) {
            simple_widget[i].hover = true;
            if (simple_widget[i].type == BUTTON && e.button.button == SDL_BUTTON_LEFT) {
              simple_widget[i].button.on = !simple_widget[i].button.on;
              simple_widget[i].button.pressed = true;
            }
            else if (e.button.button == SDL_BUTTON_RIGHT) {
              simple_widget[i].select = !simple_widget[i].select;
            }
          }
          else {
            simple_widget[i].hover = false;
          }
        }
      }
      else if (e.type == SDL_KEYUP || e.type == SDL_MOUSEBUTTONUP) {
        for (size_t i=0; i<widgets_n; ++i) {
          simple_widget[i].button.pressed = false;
        }
      }
    }
  }

}
