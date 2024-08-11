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
        DrawText(TextFormat("%fms", GetFrameTime()*1000.0f), 0, 100, 20, RED);
        if(CURR_SIMULATION_SUBSTANCE == SS_WATER) {DrawText("WATER", 0, 200, 20, BLUE);}
        else {DrawText("honey", 0, 200, 20, YELLOW);}
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
