#include<SDL3/SDL.h>
#include<stdint.h>
#include<stdbool.h>
#include<assert.h>
#include<stdio.h>
#define WIDTH 500
#define HEIGHT 500
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
typedef struct{
    Uint16 x,y;
    Uint16 length;
    Uint8 direction;
}Square;
static Square s = {
    .x = 0,
    .y = 0,
    .length = 16,
    .direction = LEFT
};
void move_one_square(Square* s,Uint8 dir);
int main(){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_LogError(SDL_LOG_PRIORITY_ERROR,"%s",SDL_GetError());
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("Snake Game",WIDTH,HEIGHT,SDL_WINDOW_RESIZABLE);
    if(!win){
        SDL_LogError(SDL_LOG_PRIORITY_ERROR,"%s",SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *ren = SDL_CreateRenderer(win,"vulkan");
    if(!ren){
        SDL_LogError(SDL_LOG_PRIORITY_ERROR,"%s",SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    SDL_Log("rendering using %s", SDL_GetRendererName(ren));
    SDL_SetRenderLogicalPresentation(ren, WIDTH,HEIGHT,SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderScale(ren, 2, 2);
    bool running = true; 
    SDL_Event event;
    Uint64 lastTick = 0;
    Uint32 frames = 0;
    while(running){
        /* event handling. */
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                running = false;
            }
            if(event.type == SDL_EVENT_KEY_DOWN){
                if(event.key.scancode == SDL_SCANCODE_W){ 
                    move_one_square(&s,UP);
                }
                if(event.key.scancode == SDL_SCANCODE_D){ 
                    move_one_square(&s,LEFT);
                }
                if(event.key.scancode == SDL_SCANCODE_A){
                    move_one_square(&s,RIGHT);
                }
                if(event.key.scancode == SDL_SCANCODE_S){
                    move_one_square(&s,DOWN);
                }
            }
        }
        /* handling drawing */
        SDL_SetRenderDrawColor(ren,0,0,0,255);/* Set the drawing color.*/
        SDL_RenderClear(ren); /* clear the previous frame*/
        /* Drawing. */
        SDL_SetRenderDrawColor(ren,100,100,100,255); /* Set the drawing color.*/
        SDL_FRect rect = {s.x,s.y,s.length,s.length}; /* define the coordinates of the Rectangular.*/
        SDL_RenderFillRect(ren,&rect); /* draw a Rectangular and fill it.*/
        SDL_RenderPresent(ren); /* present it in the window.*/
        move_one_square(&s,s.direction);
        /* frame rate calculating and handling */
        Uint64 currentTick = SDL_GetTicks();
        frames++;
        char title[50];
        snprintf(title,sizeof(title),"fps : %d",frames);
        if(currentTick > lastTick + 1000){
            SDL_SetWindowTitle(win,title);
            frames = 0;
            lastTick = currentTick;
        }
        SDL_Delay(350);
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
void move_one_square(Square* s,Uint8 dir){
    if(s->direction == LEFT && dir == RIGHT) return;
    if(s->direction == RIGHT && dir == LEFT) return;
    if(s->direction == UP && dir == DOWN) return;
    if(s->direction == DOWN && dir == UP ) return;
    if(dir == LEFT) {
        s->x += s->length;
        s->direction = dir;
    }
    if(dir == RIGHT) {
        s->x -= s->length;
        s->direction = dir;
    }
    if(dir == UP) {
        s->y -= s->length;
        s->direction = dir;
    }
    if(dir == DOWN) {
        s->y += s->length;
        s->direction = dir;
    }
}



