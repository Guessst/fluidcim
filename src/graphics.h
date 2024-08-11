#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "raylib.h"
#include "constants.h"
#include "simulation.h"

#define MIN_SCREEN_DIMENSION MIN(SCREEN_WIDTH, SCREEN_HEIGHT)
#define SQUARE_SIZE (int)(1.0f * ((float)MIN_SCREEN_DIMENSION / (float)N))
#define SQUARE_PADDING (int)(0.3f * ((float)MIN_SCREEN_DIMENSION / (float)N))

extern unsigned char squareColored[N*N];

void drawGridElement(int i, int j, Color color);
void drawGridElementWithDens(int i, int j, float densAtPos);

#endif
