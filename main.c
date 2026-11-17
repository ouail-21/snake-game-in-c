#include<SDL3/SDL.h>
#include<stdbool.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define WIDTH 500
#define HEIGHT 500
#define UP {.x = 0, .y = -1}
#define RIGHT {.x = 1, .y = 0}
#define LEFT {.x = -1, .y = 0}
#define DOWN  {.x = 0, .y = 1}
#define LENGTH 32 
typedef struct{       
    int x,y;                                   
}Vector2d;       
typedef struct{
    Vector2d dir;
    Vector2d pos;
    Uint16 length;
}Box;
typedef struct{
    Uint16 tail;
    Box snake[(WIDTH * HEIGHT)/ (LENGTH*LENGTH)];
}Snake;

void move_one_square(Box* s);
bool has_been_eaten(Box s, Box* a);
void add_box(Snake* s);
void change_position_apple(Box* a);
void draw_snake(Snake s, SDL_Renderer* ren);
void draw_apple(Box apple, SDL_Renderer* ren);
void move_snake(Snake* s);
void update_direction(Snake* s);
Vector2d Add(Vector2d v1, Vector2d v2);
Vector2d Sub(Vector2d v1, Vector2d v2);
Vector2d mult_wise(Vector2d v1, Vector2d v2);
bool is_Vector2d_equal(Vector2d v1, Vector2d v2);
bool is_collide_with_bonderies(Snake s);
bool is_collide_with_itself(Snake s);
int main(){
    srand(time(NULL));
    Box s = {
        .pos = { .x = 0, .y = 0},
        .length = LENGTH,
        .dir = RIGHT
    };

    Box apple = {
        .pos = { .x = (rand() % (HEIGHT / LENGTH)) * LENGTH, 
                 .y = (rand() % (HEIGHT / LENGTH)) * LENGTH,
        },
       .length = LENGTH,
       .dir = {0}
    };
    Snake snake = {
        .tail = 0,
        .snake[0] = s
    };
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_LogError(SDL_LOG_PRIORITY_ERROR,"%s",SDL_GetError());
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("Snake Game",
            WIDTH,HEIGHT,SDL_WINDOW_RESIZABLE);
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
    SDL_Log("rendering using %s", SDL_GetRendererName(ren));
    SDL_SetRenderLogicalPresentation(ren, WIDTH,HEIGHT,
            SDL_LOGICAL_PRESENTATION_LETTERBOX);
    bool running = true; 
    SDL_Event event;
    Uint64 lastTick = 0;
    Uint32 frames = 0;
    bool pause = false;
    Uint16 number_of_apples = 0;
    while(running){
        /* event handling. */
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                running = false;
            }
            if(event.type == SDL_EVENT_KEY_DOWN){
                if(event.key.scancode == SDL_SCANCODE_W){ 
                    if(!is_Vector2d_equal(snake.snake[0].dir, (Vector2d)DOWN))
                        snake.snake[0].dir = (Vector2d)UP;
                }
                if(event.key.scancode == SDL_SCANCODE_D){ 
                    if(!is_Vector2d_equal(snake.snake[0].dir, (Vector2d)LEFT))
                        snake.snake[0].dir = (Vector2d)RIGHT;
                }
                if(event.key.scancode == SDL_SCANCODE_A){
                    if(!is_Vector2d_equal(snake.snake[0].dir, (Vector2d)RIGHT))
                        snake.snake[0].dir = (Vector2d)LEFT;
                }
                if(event.key.scancode == SDL_SCANCODE_S){
                    if(!is_Vector2d_equal(snake.snake[0].dir, (Vector2d)UP))
                        snake.snake[0].dir = (Vector2d)DOWN;
                }
                if(event.key.scancode == SDL_SCANCODE_Y && pause){
                    pause = false;
                }
                if(event.key.scancode == SDL_SCANCODE_N && pause){
                    running = false;
                }
            }
        }
        /* Drawing. */
        if(!pause){
            SDL_SetRenderDrawColor(ren,200,201,202,255);/* Set the drawing color.*/
            SDL_RenderClear(ren); /* clear the previous frame*/
            if(has_been_eaten(snake.snake[0],&apple)){
                change_position_apple(&apple);
                add_box(&snake);
            }
            if(is_collide_with_bonderies(snake) || is_collide_with_itself(snake)){
                snake.snake[0].pos = (Vector2d){.x = 0 , .y = 0};
                number_of_apples = snake.tail;
                snake.tail = 0;
                snake.snake[0].dir = (Vector2d)RIGHT;
                pause = true;
            }
            draw_snake(snake,ren);
            draw_apple(apple,ren);
            SDL_RenderPresent(ren); /* present it in the window.*/
            move_snake(&snake);
       }
        else{
            SDL_SetRenderDrawColor(ren,0,0,0,255);/* Set the drawing color.*/
            SDL_RenderClear(ren); /* clear the previous frame*/
            SDL_SetRenderDrawColor(ren,150,60,93,255);/* Set the drawing color.*/
            char title[70];
            snprintf(title,sizeof(title), "you got %d do you want to retry? enter 'y' or 'n' to stop",number_of_apples);
            SDL_RenderDebugText(ren, 35, 250,title);
            SDL_RenderPresent(ren); /* present it in the window.*/
        }
        Uint64 currentTick = SDL_GetTicks();
        frames++;
        char title[50];
        snprintf(title,sizeof(title),"Snake game fps : %d",frames);
        if(currentTick > lastTick + 1000){
            SDL_SetWindowTitle(win,title);
            frames = 0;
            lastTick = currentTick;
        }
        SDL_Delay(250);
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
void move_one_square(Box* s){
    if(is_Vector2d_equal(s->dir,(Vector2d)RIGHT)){
        s->pos.x += s->length;
    }
    if(is_Vector2d_equal(s->dir,(Vector2d)LEFT)){
        s->pos.x -= s->length;
    }
    if(is_Vector2d_equal(s->dir,(Vector2d)UP)){
        s->pos.y -= s->length;
    }
    if(is_Vector2d_equal(s->dir,(Vector2d)DOWN)){
        s->pos.y += s->length;
    }
}
bool has_been_eaten(Box s, Box* a){
    if(is_Vector2d_equal(s.pos,a->pos)) return true;
    return false;
}
void add_box(Snake* s){
    Box pre_Box = s->snake[s->tail];
    s->tail++;
    s->snake[s->tail].pos = Sub(pre_Box.pos,
            mult_wise((Vector2d){.x = LENGTH, .y = LENGTH}, pre_Box.dir));
    s->snake[s->tail].dir = pre_Box.dir;
    s->snake[s->tail].length = LENGTH;
}

void change_position_apple(Box* a){
    a->pos.x = (rand() % (WIDTH / LENGTH)) * LENGTH;
    a->pos.y = (rand() % (HEIGHT / LENGTH)) * LENGTH;
}
void draw_snake(Snake s, SDL_Renderer* ren){
    SDL_SetRenderDrawColor(ren,100,100,100,255); /* Set the drawing color.*/
    for(size_t i = 0;i <= s.tail; i++){
        const SDL_FRect rect = {s.snake[i].pos.x, s.snake[i].pos.y,
            s.snake[i].length,s.snake[i].length}; 
        SDL_RenderFillRect(ren,&rect); /* draw a Rectangular and fill it.*/
    }
}
void move_snake(Snake* s){
    for(size_t i = 0;i <= s->tail; i++){
        move_one_square(&(s->snake[i]));
    } 
    for(size_t i = s->tail; i > 0; i--){
        s->snake[i].dir = s->snake[i-1].dir;
    }
}
Vector2d Add(Vector2d v1, Vector2d v2){
    return (Vector2d){v1.x + v2.x,v1.y + v2.y};
}

Vector2d mult_wise(Vector2d v1, Vector2d v2){
    return (Vector2d){v1.x * v2.x,v1.y * v2.y};
}
bool is_Vector2d_equal(Vector2d v1, Vector2d v2){
    if(v1.x == v2.x && v1.y == v2.y) return true;
    return false;
}
Vector2d Sub(Vector2d v1, Vector2d v2){
    return (Vector2d){v1.x - v2.x,v1.y - v2.y};
}
bool is_collide_with_bonderies(Snake s){
    if(s.snake[0].pos.y < 0 || s.snake[0].pos.y > HEIGHT || s.snake[0].pos.x < 0 || s.snake[0].pos.x > WIDTH)
        return true;
    return false;
}
void draw_apple(Box apple, SDL_Renderer* ren){
    SDL_SetRenderDrawColor(ren,200,200,100,255); /* Set the drawing color.*/
    SDL_FRect app = {apple.pos.x,apple.pos.y,apple.length,apple.length}; /* define the coordinates of the Rectangular.*/
    SDL_RenderFillRect(ren,&app); /* draw a Rectangular and fill it.*/
}
bool is_collide_with_itself(Snake s){
    for(size_t i = 1;i <= s.tail; i++){
        if(is_Vector2d_equal(s.snake[0].pos,s.snake[i].pos)){
                return true;
        }
    }
    return false;
}
    
