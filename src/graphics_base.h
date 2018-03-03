#ifndef graphics_base_h
#define graphics_base_h
#include <stdbool.h>
#include <SDL2/SDL.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

typedef struct {
	SDL_Window *window;
	SDL_GLContext glcontext;
	char *title;
	int width;
	int height;
	int flags;
} windowdata;
#define WINDOW_DATA_DEFAULT {.window=NULL,.glcontext=0,.title=NULL,.width=0,.height=0,.flags=0}

typedef struct {
	char *filename;
	GLint ref;
} shader;

typedef struct {
	shader vertex;
	shader fragment;
	GLint program;
} shaderdata;

bool create_gl_window(windowdata *wd);
void destroy_gl_window(windowdata *wd);
bool mk_shader_program(shaderdata *sd);
void rm_shader_program(shaderdata *sd);
bool create_gl_shader(shaderdata *sd, char *vs_filename, char *fs_filename);
#endif
