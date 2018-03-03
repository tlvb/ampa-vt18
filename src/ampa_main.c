#include "graphics_base.h"
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
		wd.width = 512;
		wd.height = 320;
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
  gm.vtx_n = 100;

	uint32_t previous_iteration_time = SDL_GetTicks();
  bool main_loop = true;
  while (main_loop) {
		uint32_t current_iteration_time = SDL_GetTicks();
		uint32_t dt = current_iteration_time - previous_iteration_time;

    // GL STUFF
		glBindBuffer(GL_ARRAY_BUFFER, position_buf_ref);
		glBufferData(GL_ARRAY_BUFFER, gm.vtx_n*2*sizeof(GLfloat), gm.points, GL_STATIC_DRAW);
		glVertexAttribPointer(position_ref, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, NULL);
		glEnableVertexAttribArray(position_ref);
		glBindBuffer(GL_ARRAY_BUFFER, color_buf_ref);
		glBufferData(GL_ARRAY_BUFFER, gm.vtx_n*3*sizeof(GLfloat), gm.colors, GL_STATIC_DRAW);
		glVertexAttribPointer(color_ref, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, NULL);
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
      }
    }
  }
  
}
