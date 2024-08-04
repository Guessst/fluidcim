#include "simulation.h"

#include "raylib.h"

#include "rlgl.h"
#include "raymath.h"

// #include <omp.h>
#include <stdlib.h>
#include <stdio.h>

const float WATER_VISC = 0.89;
const float WATER_DENS = 0.997;

unsigned char squareColored[N*N] = {0};
unsigned int currDebugs;

void startDebug()
{
    currDebugs = 0;
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 800;
    const int min = screenHeight < screenWidth ? screenHeight : screenWidth;

    const int squareSize = (int)(1.0f * ((float)min / (float)N));
    const int squarePadding = (int)(0.3f * ((float)min / (float)N));

    const float initialZoom = 0.5f;
    // const int squarePadding = 100;

    InitWindow(screenWidth, screenHeight, "fluidcim");

    // Image screen = LoadImageFromScreen();
    // ImageDrawRectangle(&screen, 0, 0, screen.width, screen.height, RED);

    // Texture2D texture = LoadTextureFromImage(parrots);      // Image converted to texture, uploaded to GPU memory (VRAM)
    // UnloadImage(parrots);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

    SetTargetFPS(60);
    
    Camera2D camera = { 0 };
    // TODO: calcular offset
    camera.offset = (Vector2){ screenWidth/6.0f, screenHeight/6.0f };
    camera.zoom = initialZoom;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
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

        if(IsKeyPressed(KEY_SPACE))
        {
            camera.target = Vector2Zero();
            camera.offset = (Vector2){ screenWidth/6.0f, screenHeight/6.0f };
            camera.zoom = initialZoom;
            for(int i = 0;i < N;i++)
            {
                for(int j = 0;j < N;j++)
                {
                    dens_prev[IX(i, j)] = 0;
                }
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
                        dens_prev[IX(i, j)] = WATER_DENS;
                        if(Vector2Distance(mouseWorldPos, (Vector2){posX, posY}) == squareSize*10){
                        squareColored[i*N + j] = 1;
                        }
                        // printf("%f %f\n", posX, posY);
                    }
                    else
                    {
                        squareColored[i*N + j] = 0;
                    }
                }
            }
        }
        if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
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
                        v_prev[IX(i, j)] = -v_prev[IX(i, j)];
                        squareColored[i*N + j] = 1;
                        // printf("%f %f\n", posX, posY);
                    }
                    else
                    {
                        squareColored[i*N + j] = 0;
                    }
                }
            }
        }
        

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK);


            // const char* myStr = (char*)malloc(sizeof(char)*15);
            // sprintf(myStr, "ThreadNum: %d", omp_get_max_threads());
            // DrawText(myStr, 0, 0, 10, RAYWHITE);

            // printf("posx %f, posy: %f\n", pos.x, pos.y);

            /*
                TODO

                    static Vector2 bbox = { 0.2f, 0.2f };

                    Vector2 bboxWorldMin = GetScreenToWorld2D((Vector2){ (1 - bbox.x)*0.5f*width, (1 - bbox.y)*0.5f*height }, *camera);
                    Vector2 bboxWorldMax = GetScreenToWorld2D((Vector2){ (1 + bbox.x)*0.5f*width, (1 + bbox.y)*0.5f*height }, *camera);
                    camera->offset = (Vector2){ (1 - bbox.x)*0.5f * width, (1 - bbox.y)*0.5f*height };

            */

            vel_step(u, v, u_prev, v_prev, WATER_VISC*20, GetFrameTime());//GetFrameTime());
            dens_step(dens, dens_prev, u, v, WATER_DENS, GetFrameTime());//GetFrameTime());

            BeginMode2D(camera);
                // #pragma omp parallel for
                for(int i = 0;i < N;i++)
                {
                    for(int j = 0;j < N;j++)
                    {

                        float posX =  i*squareSize + i*squarePadding;
                        float posY =  j*squareSize + j*squarePadding;
                        // if(
                        //     pos.x <= posX && posX <= pos.x + squarePadding
                        //     && pos.y <= posY && posY <= pos.y + squarePadding
                        // )
                        // {
                            float densAtPos = dens[IX(i, j)];

                            DrawRectangle(
                                posX,
                                posY,
                                squareSize,
                                squareSize,
                                squareColored[i*N + j] ? RED : (
                                    (densAtPos / WATER_DENS) <= 1.0 ?
                                        ((Color){ 255, 255, 255, 255*(densAtPos / WATER_DENS) })
                                        : BLUE
                                )
                                // (Color){ 255, 255, 255, 255*(densAtPos / 0.1)}
                            );
                        // }
                    }
                }
            EndMode2D();
        
        DrawFPS(0, 0);
        DrawText(TextFormat("%.3fms", (1.0f / GetFPS())), 0, 100, 20, RED);
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
