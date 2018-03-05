#include "util.h"
float frand(void) {
	return ((float)rand())/((float)RAND_MAX);
}
void hsv2rgb(float *r, float *g, float *b, float h, float s, float v) {
	float c = v * s;
	float m = v - c;
	h = fmodf(h+3600, 360);
	if (h < 60) { /*{{{*/
		*r = c;
		*g = c * (h/60);
		*b = 0;
	}
	else if (h < 120) {
		*r = c * (2-h/60);
		*g = c;
		*b = 0;
	}
	else if (h < 180) {
		*r = 0;
		*g = c;
		*b = c * (h/60-2);
	}
	else if (h < 240) {
		*r = 0;
		*g = c * (4-h/60);
		*b = c;

	}
	else if (h < 300) {
		*r = c * (h/60-4);
		*g = 0;
		*b = c;
	}
	else {
		*r = c;
		*g = 0;
		*b = c * (6-h/60);
	} /*}}}*/
  *r += m;
  *g += m;
  *b += m;
}
void assign_model_data(model *global, model *widget_data)
{
  widget_data->points = global->points + global->vtx_n*DIM_PER_VTX;
  widget_data->colors = global->colors + global->vtx_n*CLR_PER_VTX;
}
void alloc_model(model *gm, size_t n) { /*{{{*/
	gm->vtx_n = 0;
	gm->points = calloc(2*n, sizeof(GLfloat));
	gm->colors = calloc(3*n, sizeof(GLfloat));
} /*}}}*/
void free_model(model *gm) { /*{{{*/
	free(gm->points);
	free(gm->colors);
} /*}}}*/
