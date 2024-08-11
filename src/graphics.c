#include "graphics.h"
#include <assert.h>

unsigned char squareColored[N*N] = { 0 };

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
    if(squareColored[i*N + j])
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
