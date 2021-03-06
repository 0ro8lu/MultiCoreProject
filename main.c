#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int grid_width, grid_height;
unsigned int num_particles;
unsigned int num_iterations;
unsigned int seed_x;
unsigned int seed_y;

unsigned char not_collided(unsigned int particle_x, unsigned int particle_y, const unsigned char* grid)
{
    unsigned char my_return = 1;
    for(int y = -1; y < 2; y++)
    {
        for(int x = -1; x < 2; x++)
        {
            //Check that i'm not out of bounds
            if((particle_x == (grid_width - 1 ) && x > 0) || (particle_x == 0) && x < 0) { continue; }
            if((particle_y == (grid_height - 1) && y > 0) || (particle_y == 0) && y < 0) { continue; }

            if(x == 0 && y == 0) { continue; }
            if((particle_x + x == seed_x && particle_y + y == seed_y) ||
            (grid[(particle_x + x) + ((particle_y + y) * grid_width)] != 0)) { return 0; }
        }
    }

    return my_return;
}

unsigned int ranged_random(unsigned int lower, unsigned int upper)
{
    ///TODO: use thread safe version of random
    unsigned int my_rand = rand();
    unsigned int my_ret = my_rand % (upper - lower + 1) + lower;
    return my_ret;
}

typedef struct Particle
{
    unsigned int x_pos;
    unsigned int y_pos;
    unsigned char crystalized;
}Particle;

int main(int argc, char *argv[]) {

    if(argc < 7 || argc > 7)
    {
        printf("Usage: crystal grid_width grid_height num_particles num_iteration seed\n");
        return 1;
    }

    grid_width     = strtol(argv[1], NULL, 10);
    grid_height    = strtol(argv[2], NULL, 10);
    num_particles  = strtol(argv[3], NULL, 10);
    num_iterations = strtol(argv[4], NULL, 10);
    seed_x         = strtol(argv[5], NULL, 10);
    seed_y         = strtol(argv[6], NULL, 10);

    ///TODO: Check that seed_x and seed_y are within bounds of the grid.

    unsigned char grid[grid_width * grid_height];
    Particle particles[num_particles];

    //Init particles with random position
    for(int i = 0; i < num_particles; i++)
    {
        particles[i].x_pos = ranged_random(0, grid_width);
        particles[i].y_pos = ranged_random(0, grid_height);
        particles[i].crystalized = 0;
    }
particle_x + (particle_y * grid_width)
    srand(time(0));

    //I'm creating the grid on the stack for better performance in general: is the stack always in cache? need to research
    //Treating unsigned char as an 8 bit unsigned integer to save on space and assure better cache performance. (more elements in cache at the same time)
    for(int i = 0; i < grid_width * grid_height; i++)
    {
        grid[i] = 0;
    }

    // Main Loop
    for(int i = 0; i < num_iterations; i++)
    {
        for(int j = 0; j < num_particles; j++)
        {
            if(particles[j].crystalized == 0)
            {
                unsigned int particle_x = (particles[j].x_pos);
                unsigned int particle_y = (particles[j].y_pos);

                if(not_collided(particle_x, particle_y, (unsigned char*) &grid))
                {
                    int x_increment = (int) ranged_random(0, 2);
                    int y_increment = (int) ranged_random(0, 2);

                    //out of bounds check
                    if((particle_x == (grid_width - 1) && (x_increment - 1) > 0) || ((particle_x == 0) && (x_increment - 1) < 0))
                    {
                        x_increment = 1;
                    }

                    //out of bounds check
                    if((particle_y == (grid_height - 1) && (y_increment - 1) > 0) || ((particle_y == 0) && (y_increment - 1) < 0))
                    {
                        y_increment = 1;
                    }

                    particles[j].x_pos = particle_x + (x_increment - 1);
                    particles[j].y_pos = particle_y + (y_increment - 1);
                }
                else
                {
                    grid[particle_x + (particle_y * grid_width)]++;
                    particles[j].crystalized = 1;
                }
            }
        }
    }

    return 0;
}
