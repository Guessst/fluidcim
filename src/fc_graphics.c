#include "fc_graphics.h"
#include <stdlib.h>
#include <assert.h>

Camera2D CAMERA = { 0 };
ColoredSquare COLORED_SQUARES[N*N] = { 0 };
unsigned short NUMBER_OF_UI_ELEMENTS = 0;
UIElement* UI_ELEMENTS[MAX_NUMBER_OF_UI_ELEMENTS]; // maybe initialize with 0's would be a good idea? idk

void resetCamera()
{
    CAMERA.target = Vector2Zero();
    CAMERA.offset = (Vector2){ SCREEN_WIDTH/6.0f, SCREEN_HEIGHT/6.0f };
    CAMERA.zoom = INITIAL_ZOOM;
}

void drawGridElement(int i, int j, Color color)
{
    // int posX =  j*SQUARE_SIZE + j*SQUARE_PADDING;
    // int posY =  i*SQUARE_SIZE + i*SQUARE_PADDING;
    int posX = SQUARE_POS_X(j);
    int posY = SQUARE_POS_Y(i);

    DrawRectangle(
        posX,
        posY,
        SQUARE_SIZE,
        SQUARE_SIZE,
        color
    );
}

void drawGridElementWithDens(int i, int j, float densAtPos)
{
    Color color;
    ColoredSquare* curr = &COLORED_SQUARES[i*N + j];
    if(curr->isColored)
    {
        Color c;
        switch(curr->type)
        {   
            case CS_TRAILING:
            {
                c = RED;
                break;
            }
            case CS_DEBUG_1:
            {
                c = PURPLE;
                break;
            }
            case CS_DEBUG_2:
            {
                c = PINK;
                break;
            }
            default:
            {
                assert(0 && "not implemented");
                break;
            }
        }
        color = (Color){c.r, c.g, c.b, curr->opacity};
    }
    else
    {
        float currSimulationSubstanceDens;
        switch(CURR_SIMULATION_SUBSTANCE)
        {
            case SS_WATER:
                currSimulationSubstanceDens = WATER_DENS;
                break;
            
            case SS_HONEY:
                currSimulationSubstanceDens = HONEY_DENS;
                break;

            default:
                currSimulationSubstanceDens = 0;
                assert(0 && "unreachable");
                break;
        }

        if((densAtPos / currSimulationSubstanceDens <= 1.0))
        {
            color = (Color){
                255, 255, 255,
                (int)(255.0f*(densAtPos / currSimulationSubstanceDens))
            };
        }
        else
        {
            color = BLUE;
        }
    }

    drawGridElement(i, j, color);
}

void decayGridElementTrailing(int i, int j)
{
    ColoredSquare* curr = &COLORED_SQUARES[i*N + j];
    if(SHOULD_SIMULATE && curr->isColored && curr->type == CS_TRAILING)
    {
        curr->opacity -= 1;
        int threshold = 2*255/3;
        if(curr->opacity <= threshold) curr->isColored = 0;
    }
    
}

void drawGridArrow(int i, int j)
{
    float horizontalVelocity = u_prev_from_ui[IX(i, j)];
    float verticalVelocity = v_prev_from_ui[IX(i, j)];
    
    float r = 1.0f;
    
    if(horizontalVelocity == 0 && verticalVelocity == 0)
    {    
        return;
        int posX = (int)(SQUARE_POS_X_CENTER(j) - (r/2.0f));
        int posY = (int)(SQUARE_POS_Y_CENTER(i) - (r/2.0f));
        DrawCircle(posX, posY, r, ARROW_COLOR);
    }
    
    float amplify = 1.0f;
    float thickness = 1.5f;
    
    int xStart = (int)(SQUARE_POS_X_CENTER(j));
    // float xStart = i*SQUARE_SIZE + (SQUARE_SIZE/2) + i*SQUARE_PADDING;
    int xEnd = xStart + (int)(horizontalVelocity*amplify);

    int yStart = (int)(SQUARE_POS_Y_CENTER(i));
    // float yStart = j*SQUARE_SIZE + (SQUARE_SIZE/2) + j*SQUARE_PADDING;
    int yEnd = yStart + (int)(verticalVelocity*amplify);
    
    DrawLineEx((Vector2){xStart, yStart}, (Vector2){xEnd, yEnd}, thickness, ARROW_COLOR);
    
    float triang_size = 10.0f;
    Vector2 lineDir = (Vector2){xEnd - xStart, yEnd - yStart};
    float angle = Vector2Angle(UP, lineDir);
    drawRotatedTriangle((Vector2){xEnd, yEnd}, triang_size, angle);
}

void beginUI(void)
{
    NUMBER_OF_UI_ELEMENTS = 0;
    for(int i = 0;i < MAX_NUMBER_OF_UI_ELEMENTS;i++)
    {
        UI_ELEMENTS[i] = malloc(sizeof(UIElement));
    }
}

void addToUI(const void* var, const char* label, const VAR_TYPE type, const unsigned char isGridElement, const int i, const int j)
{
    const int posY = 100 + 20*NUMBER_OF_UI_ELEMENTS;

    UIElement* e = UI_ELEMENTS[NUMBER_OF_UI_ELEMENTS];
    e->varType = type;
    e->positionX = 0;
    e->positionY = posY;
    e->value = var;
    e->label = label;
    if(isGridElement)
    {
        e->isGridElement = isGridElement;
        e->i = i;
        e->j = j;
    }
            
    NUMBER_OF_UI_ELEMENTS += 1;
    if(NUMBER_OF_UI_ELEMENTS == MAX_NUMBER_OF_UI_ELEMENTS) assert(0 && "exceeded max number of UI elements");
}

void endUI(void)
{
    for(int i = 0;i < NUMBER_OF_UI_ELEMENTS;i++)
    {
        UIElement* e = UI_ELEMENTS[i];
        
        const char* formated = "";
        switch(e->varType)
        {
            case VT_FLOAT:
            {
                if(e->isGridElement)
                {
                    formated = TextFormat("%s(%d, %d): %f", e->label, e->i, e->j, *((float*)e->value));

                }
                else
                {
                    formated = TextFormat("%s: %f", e->label, *((float*)e->value));
                }
                break;
            }
            default:
            {
                assert(0 && "not implemented");
                break;
            }
        }
        DrawText(formated, e->positionX, e->positionY, 20, BLUE);
    }
}

void debugGridElement(int i, int j)
{
    const float* densAtPos = &dens[IX(i, j)];
    const char* label = "";

    addToUI((const void*)densAtPos, label, VT_FLOAT, 1, i, j);
}

void colorSquare(const int i, const int j, COLORED_SQUARE_TYPE type)
{
    COLORED_SQUARES[i*N + j].type = type;
    COLORED_SQUARES[i*N + j].isColored = 1;
    COLORED_SQUARES[i*N + j].opacity = 255;
}

void resetColoredSquares(void)
{
    for(int i = 0;i < N*N;i++)
    {
        COLORED_SQUARES[i].isColored = 0;
    }
}

void resetUI(void)
{
    NUMBER_OF_UI_ELEMENTS = 0;
}

void drawRotatedTriangle(Vector2 center, float size, float angle)
{
    /*
    *
    *   v1      /\
    * v2  v3   /__\
    * 
    */
    Vector2 v[3];
    Vector2 vTemp;
    Vector2 offset;
    for(int i = 0;i < 3;i++)
    {
        vTemp = Vector2Rotate(UP, angle - i*120*DEG2RAD);
        offset = (Vector2){ vTemp.x*size, vTemp.y*size };
        v[i] = (Vector2){ center.x + offset.x, center.y + offset.y };
    }
    
    DrawTriangle(v[0], v[1], v[2], ARROW_COLOR);
}
