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
    float posX =  i*SQUARE_SIZE + i*SQUARE_PADDING;
    float posY =  j*SQUARE_SIZE + j*SQUARE_PADDING;

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
                255*(densAtPos / currSimulationSubstanceDens)
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

void drawGridArrow(const int i, const int j)
{
    float horizontalVelocity = u_prev[IX(i, j)];
    float verticalVelocity = v_prev[IX(i, j)];
    
    float r = 1.0f;
    
    if(horizontalVelocity == 0 && verticalVelocity == 0)
    {    
        float posX =  SQUARE_POS_X_CENTER(j) - (r/2.0f);
        float posY =  SQUARE_POS_Y_CENTER(i) - (r/2.0f);
        // DrawCircle(posX, posY, r, RED);
        return;
    }
    
    float amplify = 10.0f;
    
    float xStart = SQUARE_POS_X_CENTER(j);
    // float xStart = i*SQUARE_SIZE + (SQUARE_SIZE/2) + i*SQUARE_PADDING;
    float xEnd = xStart + (horizontalVelocity*amplify);

    float yStart = SQUARE_POS_Y_CENTER(i);
    // float yStart = j*SQUARE_SIZE + (SQUARE_SIZE/2) + j*SQUARE_PADDING;
    float yEnd = yStart + (verticalVelocity*amplify);
    
    DrawLine(xStart, yStart, xEnd, yEnd, RED);
    // DrawLineEx((Vector2){xStart, yStart}, (Vector2){xEnd, yEnd}, 2.0f, RED);
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
    e->position = (Vector2){0, posY};
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
        
        const char* formated;
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
        DrawText(formated, e->position.x, e->position.y, 20, BLUE);
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
