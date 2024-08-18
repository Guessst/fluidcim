#include "fc_simulation.h"
#include "fc_graphics.h"
#include "fc_input.h"
#include "rlgl.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void init(void)
{
    resetCamera();
    beginUI();
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "fluidcim");

    SetTargetFPS(60);
    // TODO: properly compute offset
    init();
    while (!WindowShouldClose())
    {
        collectInput();

        if(SHOULD_SIMULATE)
        {
            float currVisc, currDens;
            switch(CURR_SIMULATION_SUBSTANCE)
            {
                case SS_WATER:
                    currVisc = WATER_VISC;
                    currDens = WATER_DENS;
                break;
                case SS_HONEY:
                    currVisc = HONEY_VISC;
                    currDens = HONEY_DENS;
                break;
                default:
                    currVisc = 0;
                    currDens = 0;
                    assert(0 && "unreachable");
                break;
            }

            vel_step(u, v, u_prev, v_prev, currVisc, GetFrameTime());
            dens_step(dens, dens_prev, u, v, currDens, GetFrameTime());
        }
        else
        {

        }

        BeginDrawing();
            ClearBackground(BLACK);
            /*
                TODO

                    static Vector2 bbox = { 0.2f, 0.2f };

                    Vector2 bboxWorldMin = GetScreenToWorld2D((Vector2){ (1 - bbox.x)*0.5f*width, (1 - bbox.y)*0.5f*height }, *camera);
                    Vector2 bboxWorldMax = GetScreenToWorld2D((Vector2){ (1 + bbox.x)*0.5f*width, (1 + bbox.y)*0.5f*height }, *camera);
                    camera->offset = (Vector2){ (1 - bbox.x)*0.5f * width, (1 - bbox.y)*0.5f*height };

            */

            // TODO: fazer vetores de velocidade

            BeginMode2D(CAMERA);
                for(int i = 0;i < N;i++)
                {
                    for(int j = 0;j < N;j++)
                    {       
                        float densAtPos = dens[IX(i, j)];
                        // printf("densAtPos: %f\n", densAtPos);
                        drawGridElementWithDens(i, j, densAtPos);
                    }
                }
            EndMode2D();
        
            DrawFPS(0, 0);
            DrawText(TextFormat("%fms", GetFrameTime()*1000.0f), 0, 20, 20, RED);
            DrawText(SHOULD_SIMULATE ? "SIMULATING" : "NOT SIMULATING", SCREEN_WIDTH / 2, 0, 20, SHOULD_SIMULATE ? GREEN : PURPLE);

            if(CURR_SIMULATION_SUBSTANCE == SS_WATER) {DrawText("WATER", 0, 200, 20, BLUE);}
            else {DrawText("honey", 0, 200, 20, YELLOW);}

            // addToUI((void*)&densAtDebugPos, "densAtDebugPos", VT_FLOAT);
            // int ssInInt = (int)SHOULD_SIMULATE;
            // addToUI((void*)&ssInInt, "shouldSimulate", VT_INT);
            // // addToUI((void*)&densAtDebugPos, "densAtDebugPos", VT_FLOAT);
            endUI();
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
