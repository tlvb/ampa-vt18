#include "graphics_base.h"
#include <assert.h>
bool create_gl_window(windowdata *wd) { /*{{{*/
	SDL_Init(SDL_INIT_VIDEO);
	// window creation {{{
	if (wd->width <= 0 || wd->height <= 0) {
		fprintf(stderr, "unreasonable window dimensions: %dx%d\n", wd->width, wd->height);
		return false;
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	wd->window = SDL_CreateWindow(
		wd->title,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		wd->width,
		wd->height,
		wd->flags | SDL_WINDOW_OPENGL
	);
	if (wd->window == NULL) {
		fprintf(stderr, "window creation error\n---\n%s\n---\n", SDL_GetError());
		return false;
	}
	// }}}
	// gl context creation {{{
	wd->glcontext = SDL_GL_CreateContext(wd->window);
	if (wd->glcontext == NULL) {
		fprintf(stderr, "gl context creation error\n---\n%s\n---\n", SDL_GetError());
		return false;
	}
	// }}}
	// gl environment setup {{{
	glViewport(0,0, (GLsizei)wd->width, (GLsizei)wd->height);
	glClearDepth(1.0f);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	//glBlendEquation(GL_ADD);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// }}}
	return true;
} /*}}}*/
void destroy_gl_window(windowdata *wd) { /*{{{*/
	if (wd->glcontext != NULL) {
		SDL_GL_DeleteContext(wd->glcontext);
	}
	if (wd->window != NULL) {
		SDL_DestroyWindow(wd->window);
	}
} /*}}}*/
static GLint mk_shader(GLenum type, const char *fn) { /*{{{*/
	// read shader source file {{{
	FILE *fd = fopen(fn, "r");
	if (fd == NULL) {
		fprintf(stderr, "null file descriptor for %s\n", fn);
		return 0;
	}
	fseek(fd, 0, SEEK_END);
	long len = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	uint8_t *buf = malloc(sizeof(uint8_t)*(len+1));
	assert(buf);
	fread(buf, sizeof(uint8_t), len, fd);
	fclose(fd);
	// }}}
	// create and compile shader from source {{{
	GLint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&buf, (GLint*)&len);
	free(buf);
	glCompileShader(shader);
	/// }}}
	// check for compile errors {{{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		fprintf(stderr, "shader (\"%s\") compilation failed:\n----------------\n", fn);
		GLint loglen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglen);
		char *log = malloc(loglen);
		assert(log);
		glGetShaderInfoLog(shader, loglen, NULL, log);
		fprintf(stderr, log);
		fprintf(stderr, "----------------\n");
		free(log);

		glDeleteShader(shader);
		return 0;
	}
	// }}}
	return shader;
} /*}}}*/
bool mk_shader_program(shaderdata *sd) { /*{{{*/
	// load and compile shader sources {{{
	sd->vertex.ref = mk_shader(GL_VERTEX_SHADER, sd->vertex.filename);
	if (sd->vertex.ref == 0) {
		return false;
	}
	sd->fragment.ref = mk_shader(GL_FRAGMENT_SHADER, sd->fragment.filename);
	if (sd->fragment.ref == 0)  {
		glDeleteShader(sd->vertex.ref);
		return false;
	}
	// }}}
	// link programs {{{
	sd->program = glCreateProgram();
	glAttachShader(sd->program, sd->vertex.ref);
	glAttachShader(sd->program, sd->fragment.ref);
	glLinkProgram(sd->program);
	// }}}
	// check for linker errors {{{
	GLint status;
	glGetProgramiv(sd->program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		fprintf(stderr, "shader linking failed:\n----------------\n");
		GLint loglen;
		glGetProgramiv(sd->program, GL_INFO_LOG_LENGTH, &loglen);
		char *log = malloc(loglen);
		assert(log);
		glGetProgramInfoLog(sd->program, loglen, NULL, log);
		fprintf(stderr, log);
		fprintf(stderr, "----------------\n");
		free(log);
		rm_shader_program(sd);
		return false;
	}
	// }}}
	return true;
} /*}}}*/
void rm_shader_program(shaderdata *sd) { /*{{{*/
	glDetachShader(sd->program, sd->vertex.ref);
	glDetachShader(sd->program, sd->fragment.ref);
	glDeleteShader(sd->vertex.ref);
	glDeleteShader(sd->fragment.ref);
	glDeleteProgram(sd->program);
	sd->vertex.ref = 0;
	sd->fragment.ref = 0;
	sd->program = 0;
} /*}}}*/
bool create_gl_shader(shaderdata *sd, char *vs_filename, char *fs_filename) {
	sd->vertex.filename = vs_filename;
	sd->fragment.filename = fs_filename;
	return mk_shader_program(sd);
}
