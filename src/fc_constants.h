#ifndef FC_CONSTANTS_H_
#define FC_CONSTANTS_H_

#define N 100
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MIN(X, Y) X > Y ? Y : X
#define UP (Vector2){0, 1}

typedef enum {
    VT_INT,
    VT_UINT,
    VT_FLOAT,
    VT_STRING,
} VAR_TYPE;

#endif
