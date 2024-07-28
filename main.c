#include "constants.h"

#include "raylib.h"

#include "rlgl.h"
#include "raymath.h"

// #include <omp.h>
#include <stdio.h>

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

    SetTargetFPS(1000);
    
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
        if (wheel != 0)
        {
            // Get the world point that is under the mouse
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            
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

            BeginMode2D(camera);
                // #pragma omp parallel for
                for(int i = 0;i < N;i++)
                    for(int j = 0;j < N;j++)
                    {
                        float posX =  i*squareSize + i*squarePadding;
                        float posY =  j*squareSize + j*squarePadding;
                        // if(
                        //     pos.x <= posX && posX <= pos.x + squarePadding
                        //     && pos.y <= posY && posY <= pos.y + squarePadding
                        // )
                        // {
                            DrawRectangle(
                                posX,
                                posY,
                                squareSize,
                                squareSize,
                                RAYWHITE
                            );
                        // }
                    }
            EndMode2D();
        
        DrawFPS(0, 0);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
