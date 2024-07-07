#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - image drawing");

    Image screen = LoadImageFromScreen();
    ImageDrawRectangle(&screen, 0, 0, screen.width, screen.height, RED);

    // Texture2D texture = LoadTextureFromImage(parrots);      // Image converted to texture, uploaded to GPU memory (VRAM)
    // UnloadImage(parrots);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

    SetTargetFPS(1000);
    //---------------------------------------------------------------------------------------

    float t = 0;
    const float maxT = 100;
    // const int pixelSquareSize = 10;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        Texture2D screenTexture = LoadTextureFromImage(screen);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        DrawFPS(0, 0);

            // ClearBackground(RAYWHITE);

            DrawTexture(screenTexture, 0, 0, WHITE);

            // int currPos = (int)(10.0 * t);
            // for(int i = currPos;i < currPos + pixelSquareSize;i++)
            // {
            //     for(int j = currPos;j < currPos + pixelSquareSize;j++)
            //     {
            //         ImageDrawPixel(&parrots, i, j, PINK);
            //     }
            // }

        EndDrawing();

        t += GetFrameTime();
        if(t >= maxT) t = 0;

        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
