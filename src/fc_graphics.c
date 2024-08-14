#include "fc_graphics.h"
#include <assert.h>

Camera2D CAMERA = { 0 };
unsigned char SQUARE_IS_COLORED[N*N] = { 0 };
unsigned short NUMBER_OF_UI_ELEMENTS = 0;

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
    if(SQUARE_IS_COLORED[i*N + j])
    {
        color = RED;
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

        if((densAtPos / (currSimulationSubstanceDens * DENS_TOLERANCE_RATIO) <= 1.0))
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

void beginUI(void)
{
    NUMBER_OF_UI_ELEMENTS = 0;
}

void addToUI(void* var, const char* label, VAR_TYPE type)
{
    switch(type)
    {
        case VT_FLOAT:
        {
            const char* formatedFloat = TextFormat("%s: %f", label, *((float*)var));
            const int posY = 100 + 20*NUMBER_OF_UI_ELEMENTS;
            DrawText(formatedFloat, 0, posY, 20, BLUE);
            break;
        }
        case VT_INT:
        {
            const char* formatedInt = TextFormat("%s: %d", label, *((int*)var));
            const int posY = 100 + 20*NUMBER_OF_UI_ELEMENTS;
            DrawText(formatedInt, 0, posY, 20, BLUE);
            break;
        }
        default:
        {
            assert(0 && "unreachable");
            break;
        }
    }

    NUMBER_OF_UI_ELEMENTS += 1;
}

void endUI(void)
{
    // TODO: Draw UI elements
}
