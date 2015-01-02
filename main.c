#include <SDL2/SDL.h>

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
#define BOARD_WIDTH 16
#define BOARD_HEIGHT 16
#define TILE_SIZE 32

int main(int argc, char*argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event evt;
    SDL_CreateWindowAndRenderer(BOARD_WIDTH * TILE_SIZE, BOARD_HEIGHT * TILE_SIZE, SDL_WINDOW_OPENGL, &window, &renderer);
    Board *board = Board_new(BOARD_WIDTH, BOARD_HEIGHT);
    Ant *ant = Ant_new(board, BOARD_WIDTH / 2, BOARD_HEIGHT / 2, NORTH);
    int running = 1;
    int start = SDL_GetTicks();
    while(running) {
        if(SDL_GetTicks() - start > 250) {
            start = SDL_GetTicks();
            Ant_step(ant);
            SDL_Delay(500);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for(int x = 0; x < board->width; x++) {
            for(int y = 0; y < board->height; y++) {
                SDL_Rect tile = {x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE};
                if(board->board[(y*board->width)+x]) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    SDL_RenderFillRect(renderer, &tile);
                }
                SDL_SetRenderDrawColor(renderer, 255,255,255,255);
                SDL_RenderDrawRect(renderer, &tile);
            }
        }
        //Draw the ant...
        SDL_Rect antRect = {
            ant->x * TILE_SIZE + TILE_SIZE / 4,
            ant->y * TILE_SIZE + TILE_SIZE / 4,
            TILE_SIZE / 2,
            TILE_SIZE / 2
        };

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &antRect);

      
        SDL_RenderPresent(renderer);
        while(SDL_PollEvent(&evt)) {
            switch(evt.type) {
                case SDL_QUIT:
                    running = 0;
                break;
            }
        }
    }
    Ant_free(ant);
    Board_free(board);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
