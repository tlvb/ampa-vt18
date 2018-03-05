#pragma once
#include <stdlib.h>
#include "graphics_base.h"

#define DIM_PER_VTX 2
#define VTX_PER_TRI 3
#define CLR_PER_VTX 3

typedef struct {
	size_t vtx_n;
	GLfloat *points;
	GLfloat *colors;
} model;
float frand(void);
void hsv2rgb(float *r, float *g, float *b, float h, float s, float v);
void assign_model_data(model *global, model *widget_data);
void alloc_model(model *gm, size_t n);
void free_global_model(model *gm);
