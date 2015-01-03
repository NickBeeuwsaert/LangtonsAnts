#include <SDL2/SDL.h>
#include "Pack.h"

typedef struct {
    char *board;
    uint32_t width;
    uint32_t height;
} Board;

Board *Board_new(int width, int height) {
    Board *b = malloc(sizeof(Board));
    b->width = width;
    b->height = height;
    b->board = malloc(sizeof(char)*b->width*b->height);
    memset(b->board, 0, sizeof(char)*b->width*b->height);
    return b;
}
void Board_free(Board*b) {
    free(b->board);
    free(b);
}
enum {NORTH, EAST, SOUTH, WEST};
typedef struct {
    Board *board;
    int x, y;
    int facing;
} Ant;

Ant *Ant_new(Board *b, int x, int y, int facing) {
    Ant *a = malloc(sizeof(Ant));
    a->board = b;
    a->x = x;
    a->y = y;
    a->facing = facing;
    return a;
}
void Ant_step(Ant *a) {
    int offset = (a->y*a->board->width)+a->x;
    //Toggle
    a->board->board[offset] = !a->board->board[offset];
    if(a->board->board[offset]) { //If at a blank space, turn right
        a->facing ++;
        if(a->facing > WEST) {
            a->facing = NORTH;
        }
    }else{ // IF not, turn left
        a->facing --;
        if(a->facing < NORTH) {
            a->facing = WEST;
        }
    }
    //Move forward
    switch(a->facing) {
        case NORTH:
            a->y--;
        break;
        case SOUTH:
            a->y++;
        break;
        case EAST:
            a->x++;
        break;
        case WEST:
            a->x--;
        break;
    }
    if(a->x < 0) a->x = a->board->width  - 1;
    if(a->y < 0) a->y = a->board->height - 1;
    if(a->x >= a->board->width)  a->x = 0;
    if(a->y >= a->board->height) a->y = 0;
}
void Ant_free(Ant *a) {
    free(a);
}

int main(int argc, char*argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event evt;
    /*Board *board = Board_new(BOARD_WIDTH, BOARD_HEIGHT);
    Ant *anta = Ant_new(board, BOARD_WIDTH / 4, BOARD_HEIGHT / 2, NORTH);
    Ant *antb = Ant_new(board, BOARD_WIDTH / 4 + BOARD_WIDTH / 2, BOARD_HEIGHT / 2, SOUTH);*/
    short board_width, board_height;
    short tile_size;
    FILE *input = fopen("struct.ant", "r");
    unpack(input, "<HHH", &board_width, &board_height, &tile_size);
    Board *board = Board_new(board_width, board_height);
    unsigned int num_ants;
    unpack(input, "<I", &num_ants);
    Ant **ants = malloc(sizeof(Ant*) * num_ants);
    for(int i = 0; i < num_ants; i++) {
        unsigned short x, y;
        char dir;
        unpack(input, "<HHb", &x, &y, &dir);
        ants[i] = Ant_new(board, x, y, dir);
    }
    fclose(input);
    SDL_CreateWindowAndRenderer(board_width * tile_size, board_height * tile_size, SDL_WINDOW_OPENGL, &window, &renderer);
    
    int running = 1;
    int start = SDL_GetTicks();
    while(running) {
        if(SDL_GetTicks() - start > 50) {
            start = SDL_GetTicks();
            for(int i = 0; i < num_ants; i++) {
                Ant_step(ants[i]);
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for(int x = 0; x < board->width; x++) {
            for(int y = 0; y < board->height; y++) {
                SDL_Rect tile = {x*tile_size, y*tile_size, tile_size, tile_size};
                if(board->board[(y*board->width)+x]) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    SDL_RenderFillRect(renderer, &tile);
                }
            }
        }
        /*
        SDL_Rect antRect;
        antRect.x = anta->x * TILE_SIZE + TILE_SIZE / 4;
        antRect.y = anta->y * TILE_SIZE + TILE_SIZE / 4;
        antRect.w = TILE_SIZE / 2;
        antRect.h = TILE_SIZE / 2;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &antRect);
        */

      
        SDL_RenderPresent(renderer);
        while(SDL_PollEvent(&evt)) {
            switch(evt.type) {
                case SDL_QUIT:
                    running = 0;
                break;
            }
        }
    }
    for(int i = 0; i < num_ants; i++) {
        Ant_free(ants[i]);
    }
    free(ants);
    //Ant_free(anta);
    Board_free(board);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
