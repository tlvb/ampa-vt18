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

  gui_widget simple_button;
  initialize_gui_widget(&gm, &simple_button, 100, 100, 50, 50, BUTTON);

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

    update_gui_widget_graphics(&simple_button, heart);
    update_fixture_widgets(simple_light, fixtures_n);

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_q) {
          main_loop = false;
        }
        else if (e.key.keysym.sym == SDLK_SPACE) {
          simple_button.select = !simple_button.select;
        }
        else if (e.key.keysym.sym == SDLK_RETURN) {
          simple_button.button.on = !simple_button.button.on;
        }
      }
      else if (e.type == SDL_MOUSEMOTION) {
        const screen_dim mx = e.motion.x;
        const screen_dim my = e.motion.y;
        if (is_inside(&simple_button.dims, mx, my)) {
          simple_button.hover = true;
        }
        else {
          simple_button.hover = false;
        }
      }
      else if (e.type == SDL_MOUSEBUTTONDOWN) {
        const screen_dim mx = e.motion.x;
        const screen_dim my = e.motion.y;
        if (is_inside(&simple_button.dims, mx, my)) {
          simple_button.hover = true;
          if (e.button.button == SDL_BUTTON_LEFT) {
            simple_button.button.on = !simple_button.button.on;
            simple_button.button.pressed = true;
          }
          else if (e.button.button == SDL_BUTTON_RIGHT) {
            simple_button.select = !simple_button.select;
          }
        }
        else {
          simple_button.hover = false;
        }
      }
      else if (e.type == SDL_KEYUP || e.type == SDL_MOUSEBUTTONUP) {
        simple_button.button.pressed = false;
      }
    }
  }

}
