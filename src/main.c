#include "simulation.h"
#include "graphics.h"

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(void)
{
    const float initialZoom = 0.5f;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "fluidcim");

    SetTargetFPS(60);
    
    Camera2D camera = { 0 };
    // TODO: properly compute offset
    camera.offset = (Vector2){ SCREEN_WIDTH/6.0f, SCREEN_HEIGHT/6.0f };
    camera.zoom = initialZoom;
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        // Translate based on mouse right click
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);

            camera.target = Vector2Add(camera.target, delta);
        }

        // Zoom based on mouse wheel
        float wheel = GetMouseWheelMove();
        // Get the world point that is under the mouse    
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        if (wheel != 0)
        {
            // Set the offset to where the mouse is
            camera.offset = GetMousePosition();

            // Set the target to match, so that the camera maps the world space point 
            // under the cursor to the screen space point under the cursor at any zoom
            camera.target = mouseWorldPos;

            // Zoom increment
            const float zoomIncrement = 0.25f;

            camera.zoom += (wheel*zoomIncrement);
            if (camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
        }
        // Reset simulation
        if(IsKeyPressed(KEY_SPACE))
        {
            camera.target = Vector2Zero();
            camera.offset = (Vector2){ SCREEN_WIDTH/6.0f, SCREEN_HEIGHT/6.0f };
            camera.zoom = initialZoom;
            
            for(int i = 0;i < sizeOfBuffer;i++)
            {
                u[i] = 0;
                v[i] = 0;
                u_prev[i] = 0;
                v_prev[i] = 0;
                dens[i] = 0;
                dens_prev[i] = 0;
            }
            for(int i = 0;i < N*N;i++)
            {
                squareColored[i] = 0;
            }
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            for(int i = 0;i < N;i++)
            {
                for(int j = 0;j < N;j++)
                {
                    float posX =  i*SQUARE_SIZE + i*SQUARE_PADDING;
                    float posY =  j*SQUARE_SIZE + j*SQUARE_PADDING;
                    if(Vector2Distance(mouseWorldPos, (Vector2){posX, posY}) <= SQUARE_SIZE)
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
                        squareColored[i*N + j] = 1;
                    }
                }
            }
        }
        if(IsKeyPressed(KEY_H))
        {
            CURR_SIMULATION_SUBSTANCE = (CURR_SIMULATION_SUBSTANCE + 1) % SS_COUNT;
        }
        

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK);
            /*
                TODO

                    static Vector2 bbox = { 0.2f, 0.2f };

                    Vector2 bboxWorldMin = GetScreenToWorld2D((Vector2){ (1 - bbox.x)*0.5f*width, (1 - bbox.y)*0.5f*height }, *camera);
                    Vector2 bboxWorldMax = GetScreenToWorld2D((Vector2){ (1 + bbox.x)*0.5f*width, (1 + bbox.y)*0.5f*height }, *camera);
                    camera->offset = (Vector2){ (1 - bbox.x)*0.5f * width, (1 - bbox.y)*0.5f*height };

            */

            // TODO: pausar simulação para debuggar sem usar breakpoint
            // TODO: fazer struct pra debugar os quadrado
            // TODO: fazer vetores de velocidade

            if(CURR_SIMULATION_SUBSTANCE == SS_WATER)
            {
                vel_step(u, v, u_prev, v_prev, WATER_VISC, GetFrameTime());
                dens_step(dens, dens_prev, u, v, WATER_DENS, GetFrameTime());
            }
            else if(CURR_SIMULATION_SUBSTANCE == SS_HONEY)
            {
                vel_step(u, v, u_prev, v_prev, HONEY_VISC, GetFrameTime());
                dens_step(dens, dens_prev, u, v, HONEY_DENS, GetFrameTime());
            }

            BeginMode2D(camera);
                for(int i = 0;i < N;i++)
                {
                    for(int j = 0;j < N;j++)
                    {       
                        float densAtPos = dens[IX(i, j)];
                        drawGridElementWithDens(i, j, densAtPos);
                    }
                }
            EndMode2D();
        
        DrawFPS(0, 0);
        DrawText(TextFormat("%fms", GetFrameTime()*1000.0f), 0, 100, 20, RED);
        if(CURR_SIMULATION_SUBSTANCE == SS_WATER) {DrawText("WATER", 0, 200, 20, BLUE);}
        else {DrawText("honey", 0, 200, 20, YELLOW);}
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
