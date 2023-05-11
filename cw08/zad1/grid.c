#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define grid_width 30
#define grid_height 30


pthread_t threads[grid_height][grid_width];

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char *src, char *dst)
{
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}

typedef struct{
    char* src;
    char* dst;
    int row;
    int column;
}CellArgs;

void handle_update(int signum, siginfo_t * info, void * abc){
    CellArgs * args = (CellArgs*) info->si_value.sival_ptr;
    args->dst[args->row * grid_width + args->column] = is_alive(args->row, args->column, args->src);
    free(args);
}

void set_signal_disposition(){
    struct sigaction sa;
    sa.sa_sigaction = handle_update;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa, NULL);
}

void * thread_work(){
    while(true){
        pause();
    }
}

void start_threads(char * src, char * dst){
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            pthread_create(&threads[i][j], NULL, thread_work, NULL);
        }
    }
}

void end_threads(){
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            pthread_kill(threads[i][j], SIGKILL);
        }
    }
}


void update_grid_using_threads(char *src, char *dst){
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            CellArgs * args = malloc(sizeof(CellArgs));
            args->src = src;
            args->dst = dst;
            args->row = i;
            args->column = j;
            union sigval val;
            val.sival_ptr = (void*) args;
            pthread_sigqueue(threads[i][j], SIGUSR1, val);
        }
    }
}