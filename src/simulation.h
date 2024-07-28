#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "constants.h"

#define IX(i,j) ((i)+(N+2)*(j))
#define SWAP(x0,x) {float* tmp = x0; x0 = x; x = tmp;}

#define sizeOfBuffer (N+2)*(N+2)

static float u[sizeOfBuffer] = { 0 };
static float v[sizeOfBuffer] = { 0 };
static float u_prev[sizeOfBuffer] = { 0 };
static float v_prev[sizeOfBuffer] = { 0 };


static float dens[sizeOfBuffer] = { 0 };
static float dens_prev[sizeOfBuffer] = { 0 };

void set_bnd (int b, float* x );
void add_source(float* x, float* s, float dt);
void diffuse(int b, float* x, float* x0, float diff, float dt);
void advect(int b, float* d, float* d0, float* u, float* v, float dt);
void dens_step(float* x, float* x0, float* u, float* v, float diff, float dt);
void vel_step(float* u, float* v, float* u0, float* v0, float visc, float dt);
void project(float* u, float* v, float* p, float* div);
#endif
