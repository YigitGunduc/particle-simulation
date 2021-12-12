#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_render.h"

#define WIDTH 800.0f
#define HEIGHT 600.0f
#define NUM_OF_PARTICLES 250
#define SIZE 2

typedef struct {
  SDL_Rect rect;
} particle;

typedef struct {
  int x, y;
} vec2;

SDL_Rect particles[NUM_OF_PARTICLES];

vec2 pos_init(int min, int max) {

  int posx = (rand() % (max - min + 1)) + min;
  int posy = (rand() % (max - min + 1)) + min;

  return (vec2) { posx, posy };
}

void delay(int numOfSec) {
  int numOfMilliSec = 1000 * numOfSec;
  time_t startTime = clock();
  while(clock() < startTime + numOfMilliSec);
}

bool isOcupied() {
  // TODO: implement func
  return false;
}

void clear(SDL_Renderer * renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderPresent(renderer);
  SDL_RenderClear(renderer);
}

void draw(SDL_Renderer * renderer, SDL_Rect * rect) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, rect);
  SDL_RenderPresent(renderer);
}

void populate(int num_of_particles) {

  int max = 200;
  int min = 200;

  for (int i = 0; i < num_of_particles; i++) {

    SDL_Rect rect; 

    vec2 pos = pos_init(min, max);

    rect.x = pos.x;
    rect.y = pos.y;
    rect.w = SIZE;
    rect.h = SIZE;

    particles[i] = rect;
  }
}

void draw_all(SDL_Renderer *renderer, int num_of_particles) {
  for (int i = 0; i < num_of_particles; i++) {
    SDL_RenderDrawRect(renderer, &particles[i]);
  }
}

void update(int num_of_particles) {
  for (int i = 0; i < num_of_particles; i++) {
    
    int min = 0;
    int max = 600;

    vec2 pos = pos_init(min, max);

    particles[i].x = pos.x; 
    particles[i].y = pos.y; 
  }
}

int main() {
  SDL_Window *window;
  SDL_Renderer *renderer;

  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow(
      "Sand Sim",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      WIDTH,
      HEIGHT,
      SDL_WINDOW_OPENGL
  );

  if (window == NULL) {
      printf("Could not create window: %s\n", SDL_GetError());
      return 1;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
      printf("Could not create window: %s\n", SDL_GetError());
      return 1;
  }

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &particles[1]);

  clear(renderer);
  
  populate(100);

  SDL_Event event;

  int quit = false;

  while (!quit) {

    SDL_Delay(10);
    SDL_PollEvent(&event);

    switch (event.type)
    {
      case SDL_QUIT:
        quit = true;
        break;
    }

    for (int iter = 0; iter < 100; iter++) {

    for (int i = 0; i < NUM_OF_PARTICLES; i++) {

      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderFillRect(renderer, &particles[i]);

    }

    SDL_RenderPresent(renderer);
    clear(renderer);

    update(NUM_OF_PARTICLES);

    delay(100);

  }

  clear(renderer);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;

  }
}
