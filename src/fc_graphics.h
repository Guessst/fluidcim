#ifndef FC_GRAPHICS_H_
#define FC_GRAPHICS_H_

#include "raylib.h"
#include "raymath.h"
#include "fc_constants.h"
#include "fc_simulation.h"

// TODO: escrever essas macros como funções pqp
#define MIN_SCREEN_DIMENSION MIN(SCREEN_WIDTH, SCREEN_HEIGHT)
#define SQUARE_SIZE (int)(1.0f * ((float)MIN_SCREEN_DIMENSION / (float)N))
#define SQUARE_PADDING (int)(0.3f * ((float)MIN_SCREEN_DIMENSION / (float)N))
#define SQUARE_INDEX_I(y) ((int)(y) / (SQUARE_SIZE + SQUARE_PADDING))
#define SQUARE_INDEX_J(x) ((int)(x) / (SQUARE_SIZE + SQUARE_PADDING))
#define SQUARE_POS_X(j) (j == 0 ? 0 : j*SQUARE_SIZE + j*SQUARE_PADDING)
#define SQUARE_POS_X_CENTER(j) ((SQUARE_POS_X(j)) + ((SQUARE_SIZE + SQUARE_PADDING)/2))
#define SQUARE_POS_Y(i) (i == 0 ? 0 : (i*SQUARE_SIZE + i*SQUARE_PADDING))
#define SQUARE_POS_Y_CENTER(i) ((SQUARE_POS_Y(i)) + ((SQUARE_SIZE + SQUARE_PADDING)/2))
#define INITIAL_ZOOM 0.5f
#define ZOOM_INCREMENT 0.25f
#define MAX_NUMBER_OF_UI_ELEMENTS 20
#define ARROW_COLOR RED

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
    int positionX;
    int positionY;
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
void decayGridElementTrailing(int i, int j);
void drawGridArrow(int i, int j);

// UI
void beginUI(void);
void addToUI(
    const void* var, const char* label, const VAR_TYPE type,
    const unsigned char isGridElement, const int i, const int j
);
void endUI(void);
void debugGridElement(int i, int j);
void colorSquare(const int i, const int j, COLORED_SQUARE_TYPE type);
void resetColoredSquares(void);
void resetUI(void);
void drawRotatedTriangle(Vector2 center, float size, float angle);
#endif
