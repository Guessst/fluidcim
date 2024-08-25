#include "fc_input.h"
#include <stdio.h>
#include <assert.h>

void collectInput(void)
{
    // For the whole scope
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), CAMERA);
        
    // Translate based on mouse right click
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/CAMERA.zoom);

        CAMERA.target = Vector2Add(CAMERA.target, delta);
    }

    float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
        // Set the offset to where the mouse is
        CAMERA.offset = GetMousePosition();

        // Set the target to match, so that the camera maps the world space point 
        // under the cursor to the screen space point under the cursor at any zoom
        CAMERA.target = mouseWorldPos;

        CAMERA.zoom += (wheel*ZOOM_INCREMENT);
        if (CAMERA.zoom < ZOOM_INCREMENT) CAMERA.zoom = ZOOM_INCREMENT;
    }
    
    if(IsKeyPressed(KEY_H))
    {
        CURR_SIMULATION_SUBSTANCE = (CURR_SIMULATION_SUBSTANCE + 1) % SS_COUNT;
        switch(CURR_SIMULATION_SUBSTANCE)
        {
            case SS_WATER:
                CURR_DENS = WATER_DENS;
                CURR_VISC = WATER_VISC;
            break;
            case SS_HONEY:
                CURR_DENS = HONEY_DENS;
                CURR_VISC = HONEY_VISC;
            break;
            default:
                assert(0 && "unreachable");
            break;
        }
    }
    if(IsKeyPressed(KEY_D))
    {
        SHOULD_SIMULATE = !SHOULD_SIMULATE;
    }
    
    if(SHOULD_SIMULATE)
    {
        // Reset simulation
        if(IsKeyPressed(KEY_SPACE))
        {
            resetCamera();
            resetSimulationVars();
            for(int i = 0;i < N*N;i++)
            {
                if(COLORED_SQUARES[i].type != CS_DEBUG_1 && COLORED_SQUARES[i].type != CS_DEBUG_2)
                {
                    COLORED_SQUARES[i].isColored = 0;
                }
            }
        }
        if(IsKeyDown(KEY_LEFT_SHIFT))
        {
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                int i = SQUARE_INDEX_I(mouseWorldPos.x);
                int j = SQUARE_INDEX_J(mouseWorldPos.y);
                u_prev_from_ui[IX(i, j)] = 10.0f;
            }
        }
        else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            for(int i = 0;i < N;i++)
            {
                for(int j = 0;j < N;j++)
                {
                    float posX =  SQUARE_POS_X_CENTER(i);
                    float posY =  SQUARE_POS_Y_CENTER(j);
                    if(Vector2Distance(mouseWorldPos, (Vector2){posX, posY}) <= SQUARE_SIZE*10)
                    {
                        switch(CURR_SIMULATION_SUBSTANCE)
                        {
                            case SS_WATER:
                                dens_prev[IX(i, j)] = WATER_DENS;
                            break;
                            case SS_HONEY:
                                dens_prev[IX(i, j)] = HONEY_DENS;
                            break;
                            default:
                                dens_prev[IX(i, j)] = 0;
                                assert(0 && "unreachable");
                            break;
                        }
                        // if(Vector2Distance(mouseWorldPos, (Vector2){posX, posY}) == squareSize*10){
                        // squareColored[i*N + j] = 1;
                        // }
                        // printf("%f %f\n", posX, posY);
                    }
                    if(IsKeyDown(KEY_D)){
                        ;
                    }
                    if((int)(mouseWorldPos.x / (SQUARE_SIZE + SQUARE_PADDING)) == i
                    && (int)(mouseWorldPos.y / (SQUARE_SIZE + SQUARE_PADDING)) == j)
                    {
                        colorSquare(i, j, CS_TRAILING);
                    }
                }
            }
        }
    }
    else // !SHOULD_SIMULATE
    {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            const int i = SQUARE_INDEX_I(mouseWorldPos.x);
            const int j = SQUARE_INDEX_J(mouseWorldPos.y);
            colorSquare(i, j, CS_DEBUG_1);
            debugGridElement(i, j);

            // addToUI((void*)densAtPos, "teste", VT_FLOAT);
        }
        if(IsKeyPressed(KEY_SPACE))
        {
            resetUI();
            resetColoredSquares();
        }
    }
}
