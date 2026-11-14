#include<SDL3/SDL.h>
#include<stdint.h>
#include<stdbool.h>
#include<assert.h>
#include<stdio.h>
#define WIDTH 500
#define HEIGHT 500

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
    SDL_Renderer *ren = SDL_CreateRenderer(win,NULL);
    if(!ren){
        SDL_LogError(SDL_LOG_PRIORITY_ERROR,"%s",SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderLogicalPresentation(ren, WIDTH,HEIGHT,SDL_LOGICAL_PRESENTATION_STRETCH);
    bool running = true; 
    SDL_Event event;
    SDL_FRect rect_screen = {0,0,WIDTH,HEIGHT};
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
                }
                if(event.key.scancode == SDL_SCANCODE_D){ 
                }
                if(event.key.scancode == SDL_SCANCODE_A){
                }
                if(event.key.scancode == SDL_SCANCODE_S){
                }
            }
        /* handling drawing */
        SDL_SetRenderDrawColor(ren,0,0,0,255);/* Set the drawing color.*/
        SDL_RenderClear(ren); /* clear the previous frame*/
        /* Drawing. */
        SDL_SetRenderDrawColor(ren,100,100,100,255); /* Set the drawing color.*/
        SDL_FRect rect = {0,0,49,38}; /* define the coordinates of the Rectangular.*/
        SDL_RenderFillRect(ren,&rect); /* draw a Rectangular and fill it.*/
        SDL_RenderPresent(ren); /* present it in the window.*/
        }
        
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
        SDL_Delay(32);
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
