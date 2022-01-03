#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
#include "../includes/SDL/include/SDL.h"
#include "../includes/SDL/include/SDL_rect.h"
#include "../includes/SDL/include/SDL_render.h"


#define SIZE 3
#define WIDTH 200.0f
#define HEIGHT 200.0f
#define NUM_OF_PARTICLES 1000

typedef struct {
  int x, y;
} vec2;

typedef struct {
  bool bottom;
  bool side1;
  bool side2;
} available_positions;

SDL_Rect particles[NUM_OF_PARTICLES];


extern inline void delay(int numOfSec) {
  int numOfMilliSec = 1000 * numOfSec;
  time_t startTime = clock();
  while(clock() < startTime + numOfMilliSec);
}

vec2 pos_init(int xmin, int xmax, int ymin, int ymax) {

  int posx = (rand() % (xmax - xmin + 1)) + xmin;
  int posy = (rand() % (ymax - ymin + 1)) + ymin;

  return (vec2) { posx, posy };
}

void clear(SDL_Renderer * renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderPresent(renderer);
  SDL_RenderClear(renderer);
}

void SDL_quit(SDL_Renderer * renderer, SDL_Window *window) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void draw(SDL_Renderer * renderer) {
    for (int i = 0; i < NUM_OF_PARTICLES; i++) {
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(renderer, 194, 178, 128, 128);
      SDL_RenderFillRect(renderer, &particles[i]);
  }
}

void populate() {

  int xmax = 30;
  int xmin = 80;
  int ymin = -40;
  int ymax = 150;

  for (int i = 0; i < NUM_OF_PARTICLES; i++) {

    SDL_Rect rect; 

    vec2 pos = pos_init(xmin, xmax, ymin, ymax);

    rect.x = pos.x;
    rect.y = pos.y;
    rect.w = SIZE;
    rect.h = SIZE;

    particles[i] = rect;
  }
}

available_positions isOcupied(int x, int y) {

  available_positions res = { .bottom = true, .side1 = true, .side2 = true };

  for (int i = 0; i < NUM_OF_PARTICLES; i++) {
    if (particles[i].y == y + SIZE && particles[i].x == x) {
      res.bottom = false;
    } else if (particles[i].x == x + SIZE && y == particles[i].y) {
      res.side1 = false;
    }  else if (particles[i].x == x - SIZE  && y == particles[i].y) {
      res.side2 = false;
    }
  }

  return res;
}

void update() {
  srand(time(NULL));

  for (int i = 0; i < NUM_OF_PARTICLES; i++) {
     
    available_positions pos = isOcupied(particles[i].x, particles[i].y);

    int r = rand();
   
    if (pos.bottom && particles[i].y <= 170) {
      particles[i].x = particles[i].x; 
      particles[i].y = particles[i].y + SIZE;
    } else if (pos.side1 && (r % 2) == 0) {
        particles[i].x = particles[i].x + SIZE; 
        particles[i].y = particles[i].y; 
    } else if (pos.side2 && (r % 2) == 1) {
        particles[i].x = particles[i].x - SIZE; 
        particles[i].y = particles[i].y; 
    } else {
        particles[i].x = particles[i].x; 
        particles[i].y = particles[i].y; 
    }
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
      exit(1);
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
      printf("Could not create renderer: %s\n", SDL_GetError());
      exit(1);
  }

  clear(renderer);
  
  populate();

  SDL_Event event;

  bool quit = false;

  while (!quit) {

    SDL_Delay(10);
    SDL_PollEvent(&event);

    for (int iter = 0; iter < 300; iter++) {

      switch (event.type)
      {
        case SDL_QUIT:
          quit = true;
          break;
      }

      draw(renderer);
      SDL_RenderPresent(renderer);
      clear(renderer);
      update();
      delay(100);
    }

  clear(renderer);

  SDL_quit(renderer, window);

  return 0;

  }
}
