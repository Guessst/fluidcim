#include "fc_velocity_presets.h"
#include "fc_simulation.h"
#include "fc_graphics.h"
#include "raymath.h"

// PATTERN 1: velocity vectors all pointing to the center of the grid
void setPreset1(int radiusInGridElements)
{
    float amplifyVel = 50.0f;
    Vector2 gridCenter = (Vector2){N/2, N/2};

    for(int i = 0;i < N;i++)
    {
        for(int j = 0;j < N;j++)
        {
            //if(Vector2Distance(mouseWorldPos, (Vector2){posX, posY}) <= SQUARE_SIZE*10)
            Vector2 currElement = (Vector2){ j, i };
            if(Vector2Distance(gridCenter, currElement) <= radiusInGridElements)
            {
                // TODO: seila tenho que pensar melhor em como apontar as setas pro centro ja que estão passando do centro
                COLORED_SQUARES[i*N + j] = (ColoredSquare){ 1, CS_DEBUG_2, 255 };
                Vector2 dirToCenter = (Vector2){ gridCenter.x - currElement.x, gridCenter.y - currElement.y };
                u_prev_from_ui[IX(i, j)] = dirToCenter.x * amplifyVel;
                v_prev_from_ui[IX(i, j)] = dirToCenter.y * amplifyVel;
            }
        }
    }
}