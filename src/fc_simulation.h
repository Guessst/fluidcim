#ifndef FC_SIMULATION_H_
#define FC_SIMULATION_H_

#include "fc_constants.h"

#define IX(i,j) ((i)+(N+2)*(j))
#define SWAP(x0,x) {float* tmp = x0; x0 = x; x = tmp;}

#define sizeOfBuffer (N+2)*(N+2)

#define WATER_VISC 0.89
#define WATER_DENS 0.997

#define HONEY_VISC 10000*WATER_VISC
#define HONEY_DENS 1.4*WATER_DENS

#define DENS_TOLERANCE_PERCENTAGE 10.0f
#define DENS_TOLERANCE_RATIO (1+(DENS_TOLERANCE_PERCENTAGE/100.0f))

#define CAP_DENS true

typedef enum {
    SS_WATER,
    SS_HONEY,
    SS_COUNT
} SIMULATION_SUBSTANCE;

extern float u[sizeOfBuffer];
extern float v[sizeOfBuffer];
extern float u_prev[sizeOfBuffer];
extern float v_prev[sizeOfBuffer];

extern float dens[sizeOfBuffer];
extern float dens_prev[sizeOfBuffer];

extern SIMULATION_SUBSTANCE CURR_SIMULATION_SUBSTANCE;
extern unsigned char SHOULD_SIMULATE;

void set_bnd (int b, float* x );
void add_source(float* x, float* s, float dt);
void diffuse(int b, float* x, float* x0, float diff, float dt);
void advect(int b, float* d, float* d0, float* u, float* v, float dt);
void dens_step(float* x, float* x0, float* u, float* v, float diff, float dt);
void vel_step(float* u, float* v, float* u0, float* v0, float visc, float dt);
void project(float* u, float* v, float* p, float* div);
#endif
