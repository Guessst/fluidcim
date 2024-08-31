#include "fc_simulation.h"

float u[sizeOfBuffer] = {0};
float v[sizeOfBuffer] = {0};
float u_prev[sizeOfBuffer] = {0};
float v_prev[sizeOfBuffer] = {0};
float u_prev_from_ui[sizeOfBuffer] = {0};
float v_prev_from_ui[sizeOfBuffer] = {0};

float dens[sizeOfBuffer] = {0};
float dens_prev[sizeOfBuffer] = {0};

float CURR_DENS = WATER_DENS;
float CURR_VISC = WATER_VISC;

SIMULATION_SUBSTANCE CURR_SIMULATION_SUBSTANCE = SS_WATER;
unsigned char SHOULD_SIMULATE = 0;

void set_bnd (int b, float* x )
{
    for (int i=1;
        i <= N;
        i++)
    {
        x[IX(0    , i)] = b==1 ? -x[IX(1, i)] : x[IX(1, i)];
        x[IX(N + 1, i)] = b==1 ? -x[IX(N, i)] : x[IX(N, i)];
        x[IX(i    , 0)] = b==2 ? -x[IX(i, 1)] : x[IX(i, 1)];
        x[IX(i, N + 1)] = b==2 ? -x[IX(i, N)] : x[IX(i, N)];
    }

    x[IX(0    , 0    )] = 0.5f*(x[IX(1, 0    )] + x[IX(0    , 1)]);
    x[IX(0    , N + 1)] = 0.5f*(x[IX(1, N + 1)] + x[IX(0    , N)]);
    x[IX(N + 1, 0    )] = 0.5f*(x[IX(N, 0    )] + x[IX(N + 1, 1)]);
    x[IX(N + 1, N + 1)] = 0.5f*(x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}

void add_source(float* x, float* s, float dt)
{
    for(int i=0;
        i < sizeOfBuffer;
        i++)
    {
        x[i] += dt*s[i];
    }
}

void diffuse(int b, float* x, float* x0, float diff, float dt)
{
    float a = dt * diff * N * N;
    for(int k = 0;
        k < 20;
        k++)
    {
        for(int i = 1;
            i <= N;
            i++)
        {
            for(int j = 1;
                j <= N;
                j++)
            {
                x[IX(i,j)] =
                    (
                        x0[IX(i,j)]
                        + a*(x[IX(i - 1,j    )]
                        +    x[IX(i + 1,j    )]
                        +    x[IX(i    ,j - 1)]
                        +    x[IX(i    ,j + 1)])
                    ) / (1 + 4*a);
            }
        }
        set_bnd ( b, x );
    }
}

void advect(int b, float* d, float* d0, float* u, float* v, float dt )
{
    int i0, j0, i1, j1;
    float x, y, s0, t0, s1, t1, dt0;
    dt0 = dt*N;
    
    for(int i = 1;
        i <= N;
        i++)
    {
        for(int j = 1;
            j <= N;
            j++)
        {
            x = i - dt0*u[IX(i, j)];
            y = j - dt0*v[IX(i, j)];
            
            if(x < 0.5  ) x =     0.5;
            if(x > N+0.5) x = N + 0.5;
            i0 = (int)x;
            i1 = i0 + 1;
            
            if(y <     0.5) y =     0.5;
            if(y > N + 0.5) y = N + 0.5;
            j0 = (int)y;
            j1 = j0 + 1;
            
            s1 = x - i0;
            s0 = 1 - s1;
            t1 = y - j0;
            t0 = 1 - t1;
            
            d[IX(i, j)] =
            s0*(
                t0*d0[IX(i0, j0)] + t1*d0[IX(i0, j1)]
            )
            +
            s1*(
                t0*d0[IX(i1, j0)] + t1*d0[IX(i1, j1)]
            );
        }
    }
    set_bnd ( b, d );
}

void dens_step_any(float* x, float* x0, float* u, float* v, float diff, float dt)
{
    add_source(x, x0, dt);
    
    SWAP(x0, x);
    diffuse(0, x, x0, diff, dt);

    SWAP(x0, x);
    advect(0, x, x0, u, v, dt);
}

void dens_step(float* x, float* x0, float* u, float* v, float dt)
{
    dens_step_any(x, x0, u, v, CURR_DENS, dt);
}

void vel_step_any(float* u, float* v, float* u0, float* v0, float visc, float dt)
{
    add_source (u, u0, dt );
    add_source (v, v0, dt );
    
    SWAP ( u0, u ); diffuse (1, u, u0, visc, dt );
    SWAP ( v0, v ); diffuse (2, v, v0, visc, dt );
    
    project (u, v, u0, v0 );
    
    SWAP ( u0, u ); SWAP ( v0, v );
    
    advect (1, u, u0, u0, v0, dt );
    advect (2, v, v0, u0, v0, dt );
    
    project (u, v, u0, v0 );
}

void vel_step(float* u, float* v, float* u0, float* v0, float dt)
{
    vel_step_any(u, v, u0, v0, CURR_VISC, dt);
}

void project(float* u, float* v, float* p, float* div)
{
    float h = 1.0f/N;
    for(int i = 1;
        i <= N;
        i++)
    {
        for(int j = 1;
            j <= N;
            j++)
        {
            div[IX(i, j)] =
            -0.5f*h*(
                u[IX(i + 1,j    )] - u[IX(i - 1, j   )]
                +
                v[IX(i    ,j + 1)] - v[IX(i    ,j - 1)]
            );

            p[IX(i,j)] = 0;
        }
    }
    set_bnd ( 0, div );
    set_bnd ( 0, p );
    
    for(int k = 0;
        k < 20;
        k++)
    {
        for(int i = 1;
            i <= N;
            i++)
        {
            for(int j = 1;
                j <= N;
                j++)
            {
                p[IX(i,j)] =
                (
                    div[IX(i    , j    )]
                    + p[IX(i - 1, j    )]
                    + p[IX(i + 1, j    )]
                    + p[IX(i    , j - 1)]
                    + p[IX(i    , j + 1)]
                )/4;
            }
        }
        set_bnd ( 0, p );
    }
    for(int i = 1;
        i <= N;
        i++)
    {
        for(int j = 1;
            j <= N;
            j++)
        {
            u[IX(i,j)] -= 0.5f*(p[IX(i + 1, j    )] - p[IX(i - 1,j    )])/h;
            v[IX(i,j)] -= 0.5f*(p[IX(i    , j + 1)] - p[IX(i    ,j - 1)])/h;
        }
    }
    set_bnd ( 1, u );
    set_bnd ( 2, v );
}

void resetSimulationVars(void)
{
    for(int i = 0;i < sizeOfBuffer;i++)
    {
        u[i] = 0;
        v[i] = 0;
        u_prev[i] = 0;
        v_prev[i] = 0;
        // u_prev_from_ui[i] = 0;
        // v_prev_from_ui[i] = 0;
        dens[i] = 0;
        dens_prev[i] = 0;
    }
}

