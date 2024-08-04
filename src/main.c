#include "simulation.h"

#include "raylib.h"

#include "rlgl.h"
#include "raymath.h"

#include <stdlib.h>
#include <stdio.h>

#define WATER_VISC 0.89
#define WATER_DENS 0.997

#define HONEY_VISC 10000*WATER_VISC
#define HONEY_DENS 1.4*WATER_DENS

unsigned char flagWaterHoney = 0;

const int minScreenDimension = MIN(SCREEN_WIDTH, SCREEN_HEIGHT);
const int squareSize = (int)(1.0f * ((float)minScreenDimension / (float)N));
const int squarePadding = (int)(0.3f * ((float)minScreenDimension / (float)N));
unsigned char squareColored[N*N] = {0};

void drawGridElement(int i, int j, Color color)
{

    float posX =  i*squareSize + i*squarePadding;
    float posY =  j*squareSize + j*squarePadding;

    DrawRectangle(
        posX,
        posY,
        squareSize,
        squareSize,
        color
        // (Color){ 255, 255, 255, 255*(densAtPos / 0.1)}
    );
}

int main(void)
{

    // Initialization
    //--------------------------------------------------------------------------------------

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
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            for(int i = 0;i < N;i++)
            {
                for(int j = 0;j < N;j++)
                {
                    float posX =  i*squareSize + i*squarePadding;
                    float posY =  j*squareSize + j*squarePadding;
                    if(Vector2Distance(mouseWorldPos, (Vector2){posX, posY}) <= squareSize*10)
                    {
                        dens_prev[IX(i, j)] = (flagWaterHoney ? WATER_DENS : HONEY_DENS);
                        // if(Vector2Distance(mouseWorldPos, (Vector2){posX, posY}) == squareSize*10){
                        // squareColored[i*N + j] = 1;
                        // }
                        // printf("%f %f\n", posX, posY);
                    }
                    else
                    {
                        squareColored[i*N + j] = 0;
                    }
                }
            }
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
        {
            for(int i = 0;i < N;i++)
            {
                for(int j = 0;j < N;j++)
                {
                    float posX =  i*squareSize + i*squarePadding;
                    float posY =  j*squareSize + j*squarePadding;
                    if(Vector2Distance(mouseWorldPos, (Vector2){posX, posY}) <= squareSize*10)
                    {

                        // dens_prev[IX(i, j)] = 0;
                        // dens[IX(i, j)] = 0;
                        // v[IX(i, j)] = 0;
                        // dens_prev[IX(i, j)] = 0;
                        if(dens_prev[IX(i, j)] > 0) dens_prev[IX(i, j)] = 0;
                        // u_prev[IX(i, j)] = WATER_VISC;
                        
                        
                        // TODO: modificar u_prev 



                        // squareColored[i*N + j] = 1;
                        // printf("%f %f\n", posX, posY);
                    }
                    else
                    {
                        squareColored[i*N + j] = 0;
                    }
                }
            }
        }
        if(IsKeyPressed(KEY_H))
        {
            flagWaterHoney = !flagWaterHoney;
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

            if(flagWaterHoney)
            {
                vel_step(u, v, u_prev, v_prev, WATER_VISC, GetFrameTime());
                dens_step(dens, dens_prev, u, v, WATER_DENS, GetFrameTime());
            }
            else
            {
                vel_step(u, v, u_prev, v_prev, HONEY_VISC, GetFrameTime());
                dens_step(dens, dens_prev, u, v, HONEY_DENS, GetFrameTime());
            }

            BeginMode2D(camera);
                for(int i = 0;i < N;i++)
                {
                    for(int j = 0;j < N;j++)
                    {
                            // if((i + j)%2 == 0) u_prev[IX(i, j)] = 10;

                            float densAtPos = dens[IX(i, j)];
                            float uPrev = u_prev[IX(i, j)];
                            // float vAtPos = v[IX(i, j)];
                            if(flagWaterHoney)
                            {
                                drawGridElement(
                                    i,
                                    j,
                                    squareColored[i*N + j] ?
                                        RED 
                                        : (
                                            (densAtPos / WATER_DENS) <= 1.0 ?
                                                ((Color){ 255, 255, 255, 255*(densAtPos / WATER_DENS) })
                                                : BLUE
                                        )
                                );
                            }
                            else
                            {
                                drawGridElement(
                                    i,
                                    j,
                                    squareColored[i*N + j] ?
                                        RED 
                                        : (
                                            (densAtPos / HONEY_DENS) <= 1.0 ?
                                                ((Color){ 255, 255, 255, 255*(densAtPos / HONEY_DENS) })
                                                : BLUE
                                        )
                                );
                            }
                            const Color transparentOrange = (Color){ 255, 161, 0, 255/2 };
                            // if(uPrev >= WATER_VISC) drawGridElement(i, j, ORANGE);
                    }
                }
            EndMode2D();
        
        DrawFPS(0, 0);
        DrawText(TextFormat("%fms", GetFrameTime()*1000.0f), 0, 100, 20, RED);
        if(flagWaterHoney) {DrawText("WATER", 0, 200, 20, BLUE);}
        else {DrawText("honey", 0, 200, 20, YELLOW);}
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
