#ifndef FC_GRAPHICS_H_
#define FC_GRAPHICS_H_

#include "raylib.h"
#include "raymath.h"
#include "fc_constants.h"
#include "fc_simulation.h"

#define MIN_SCREEN_DIMENSION MIN(SCREEN_WIDTH, SCREEN_HEIGHT)
#define SQUARE_SIZE (int)(1.0f * ((float)MIN_SCREEN_DIMENSION / (float)N))
#define SQUARE_PADDING (int)(0.3f * ((float)MIN_SCREEN_DIMENSION / (float)N))
#define INITIAL_ZOOM 0.5f
#define ZOOM_INCREMENT 0.25f

extern Camera2D CAMERA;
extern unsigned char SQUARE_IS_COLORED[N*N];

void resetCamera(void);
void drawGridElement(int i, int j, Color color);
void drawGridElementWithDens(int i, int j, float densAtPos);

#endif
