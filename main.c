#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_render.h"

#define WIDTH 200.0f
#define HEIGHT 200.0f
#define SIZE 2
#define STEP 1
#define NUM_OF_PARTICLES 1000

typedef struct {
  SDL_Rect rect;
} particle;

typedef struct {
  int x, y;
} vec2;

SDL_Rect particles[NUM_OF_PARTICLES];

vec2 pos_init(int xmin, int xmax, int ymin, int ymax) {

  int posx = (rand() % (xmax - xmin + 1)) + xmin;
  int posy = (rand() % (ymax - ymin + 1)) + ymin;

  return (vec2) { posx, posy };
}

void delay(int numOfSec) {
  int numOfMilliSec = 1000 * numOfSec;
  time_t startTime = clock();
  while(clock() < startTime + numOfMilliSec);
}

typedef struct {
  bool bottom;
  bool side1;
  bool side2;
} available_positions;

available_positions isOcupied(int x, int y) {
  // TODO: implement func

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

void clear(SDL_Renderer * renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderPresent(renderer);
  SDL_RenderClear(renderer);
}

void draw(SDL_Renderer * renderer, SDL_Rect * rect) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, rect);
  SDL_RenderPresent(renderer);
}

void populate(int num_of_particles) {

  int xmax = 10;
  int xmin = 50;
  int ymin = 0;
  int ymax = 150;

  for (int i = 0; i < num_of_particles; i++) {

    SDL_Rect rect; 

    vec2 pos = pos_init(xmin, xmax, ymin, ymax);

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
  srand(time(NULL));

  for (int i = 0; i < num_of_particles; i++) {
     
    available_positions pos = isOcupied(particles[i].x, particles[i].y);

    int r = rand();
   
    if (pos.bottom && particles[i].y <= 150) {
      particles[i].x = particles[i].x; 
      particles[i].y = particles[i].y + 2;
    } else if (pos.side1 && (r % 2) == 0) {
        particles[i].x = particles[i].x + 2; 
        particles[i].y = particles[i].y; 
    } else if (pos.side2 && (r % 2) == 1) {
        particles[i].x = particles[i].x - 2; 
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
      return 1;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
      printf("Could not create window: %s\n", SDL_GetError());
      return 1;
  }

  clear(renderer);
  
  populate(NUM_OF_PARTICLES);

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


    for (int iter = 0; iter < 300; iter++) {
      for (int i = 0; i < NUM_OF_PARTICLES; i++) {
        SDL_SetRenderDrawColor(renderer, 194, 178, 128, 255);
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
