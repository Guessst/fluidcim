#ifndef FC_GRAPHICS_H_
#define FC_GRAPHICS_H_

#include "raylib.h"
#include "raymath.h"
#include "fc_constants.h"
#include "fc_simulation.h"

#define MIN_SCREEN_DIMENSION MIN(SCREEN_WIDTH, SCREEN_HEIGHT)
#define SQUARE_SIZE (int)(1.0f * ((float)MIN_SCREEN_DIMENSION / (float)N))
#define SQUARE_PADDING (int)(0.3f * ((float)MIN_SCREEN_DIMENSION / (float)N))
#define SQUARE_INDEX_I(x) (((int)x) / (SQUARE_SIZE + SQUARE_PADDING))
#define SQUARE_INDEX_J(y) (((int)y) / (SQUARE_SIZE + SQUARE_PADDING))
#define INITIAL_ZOOM 0.5f
#define ZOOM_INCREMENT 0.25f
#define MAX_NUMBER_OF_UI_ELEMENTS 20

typedef enum {
    CS_TRAILING = 0,
    CS_DEBUG_1,
    CS_DEBUG_2,
    CS_COUNT
} COLORED_SQUARE_TYPE;

typedef struct ColoredSquare {
    unsigned char isColored;
    COLORED_SQUARE_TYPE type;
    int opacity;
} ColoredSquare;

typedef struct UIElement {
    VAR_TYPE varType;
    Vector2 position;
    const void* value;
    const char* label;
    // for grid elements
    unsigned char isGridElement;
    int i;
    int j;
} UIElement;


extern Camera2D CAMERA;
extern ColoredSquare COLORED_SQUARES[N*N];
extern unsigned short NUMBER_OF_UI_ELEMENTS;
extern UIElement* UI_ELEMENTS[MAX_NUMBER_OF_UI_ELEMENTS];

// Camera
void resetCamera(void);

// Grid
void drawGridElement(int i, int j, Color color);
void drawGridElementWithDens(int i, int j, float densAtPos);

// UI
void beginUI(void);
void addToUI(
    const void* var, const char* label, const VAR_TYPE type,
    const unsigned char isGridElement, const int i, const int j
);
void endUI(void);
void debugGridElement(int i, int j);
void colorSquare(const int i, const int j, COLORED_SQUARE_TYPE type);
#endif
