#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_render.h"

#define SIZE 3
#define WIDTH 200.0f
#define HEIGHT 200.0f 
#define NUM_OF_ITER 300
#define NUM_OF_SAND_PARTICLES 1000
#define NUM_OF_WATER_PARTICLES 1000

typedef struct {
  int x, y;
} vec2;

typedef struct {
  unsigned char r, g, b;
  int a;
} color;

enum p_types{
 AIR=0,
 SAND=1,
 WATER=2,
};

typedef struct {
  color Color;
  int type;
  SDL_Rect rect;
} particle;

typedef struct {
  bool bottom;
  bool side1;
  bool side2;
  bool diag1;
  bool diag2;
  int top_index;
  bool top;
} available_positions;

particle particles[NUM_OF_SAND_PARTICLES + NUM_OF_WATER_PARTICLES];

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
  SDL_SetRenderDrawColor(renderer, 0, 30, 30, 255);
  SDL_RenderPresent(renderer);
  SDL_RenderClear(renderer);
}

void SDL_quit(SDL_Renderer * renderer, SDL_Window *window) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

available_positions isOcupied(int x, int y) {

  available_positions res = { .bottom = true, 
                              .side1 = true, 
                              .side2 = true,
                              .diag1 = true,
                              .diag2 = true,
                              .top = false,
                              .top_index = -1};

  for (int i = 0; i < NUM_OF_SAND_PARTICLES + NUM_OF_WATER_PARTICLES; i++) {
    if (particles[i].rect.y == y + SIZE && particles[i].rect.x == x) {
      res.bottom = false;
    } else if (particles[i].rect.x == x + SIZE && y == particles[i].rect.y) {
      res.side1 = false;
    }  else if (particles[i].rect.x == x - SIZE  && y == particles[i].rect.y) {
      res.side2 = false;
    }  else if (particles[i].rect.x == x + SIZE  && y == particles[i].rect.y + SIZE) {
      res.diag1 = false;
    }  else if (particles[i].rect.x == x - SIZE  && y == particles[i].rect.y + SIZE) {
      res.diag2 = false;
    }  else if (particles[i].rect.x == x && y == particles[i].rect.y - SIZE) {
      res.top = true;
      res.top_index = i;
    }
  }

  return res;
}

void draw(SDL_Renderer * renderer) {
    for (int i = 0; i < NUM_OF_SAND_PARTICLES + NUM_OF_WATER_PARTICLES; i++) {
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(renderer,
                             particles[i].Color.r, 
                             particles[i].Color.g, 
                             particles[i].Color.b, 
                             particles[i].Color.a);
      SDL_RenderFillRect(renderer, &particles[i].rect);
  }
}

void populate() {

  int index = 0;

  for (index = 0; index < NUM_OF_SAND_PARTICLES + NUM_OF_WATER_PARTICLES; index++) {
    if (index < NUM_OF_WATER_PARTICLES) {

      int xmax = 40;
      int xmin = 80;
      int ymin = -40;
      int ymax = 150;

      particle temp;

      vec2 pos = pos_init(xmin, xmax, ymin, ymax);

      temp.rect.x = pos.x;
      temp.rect.y = pos.y;
      temp.rect.w = SIZE;
      temp.rect.h = SIZE;

      temp.Color = (color) { 194, 178, 128, 170 };
      temp.type = SAND;

      particles[index] = temp;
    }

    if (index > NUM_OF_WATER_PARTICLES) {

      int xmin = 10;
      int xmax = 190;
      int ymin = 150;
      int ymax = 170 - SIZE;

      particle temp;

      vec2 pos = pos_init(xmin, xmax, ymin, ymax);

      temp.rect.x = pos.x;
      temp.rect.y = pos.y;
      temp.rect.w = SIZE;
      temp.rect.h = SIZE;

      temp.Color = (color) { 10, 140, 238, 30 };
      temp.type = WATER;

      particles[index] = temp;

    }
  }

}

void update() {
  srand(time(NULL));

  for (int i = 0; i < NUM_OF_SAND_PARTICLES + NUM_OF_WATER_PARTICLES; i++) {
     
    available_positions pos = isOcupied(particles[i].rect.x, particles[i].rect.y);

   
    if (particles[i].type == SAND) {

      int r = rand();

      if (pos.bottom && particles[i].rect.y <= 170) {
        particles[i].rect.x = particles[i].rect.x; 
        particles[i].rect.y = particles[i].rect.y + SIZE;
      } else if (pos.side1 && (r % 2) == 0) {
          particles[i].rect.x = particles[i].rect.x + SIZE; 
          particles[i].rect.y = particles[i].rect.y; 
      } else if (pos.side2 && (r % 2) == 1) {
          particles[i].rect.x = particles[i].rect.x - SIZE; 
          particles[i].rect.y = particles[i].rect.y; 
      } else {
          particles[i].rect.x = particles[i].rect.x; 
          particles[i].rect.y = particles[i].rect.y; 
      }

    }
    if (particles[i].type == WATER) {

      int r = rand() % 100 + 1;

      if (pos.bottom && particles[i].rect.y <= 170) {
        particles[i].rect.x = particles[i].rect.x; 
        particles[i].rect.y = particles[i].rect.y + SIZE;
      } else if (pos.side1 &&  r <= 25 && particles[i].rect.y <= 170 )  {
          particles[i].rect.x = particles[i].rect.x + SIZE; 
          particles[i].rect.y = particles[i].rect.y; 
      } else if (pos.side2 && r <= 50 && particles[i].rect.y <= 170) {
          particles[i].rect.x = particles[i].rect.x - SIZE; 
          particles[i].rect.y = particles[i].rect.y; 
      } else if (pos.diag1 && r <= 75 && particles[i].rect.y <= 170) {
          particles[i].rect.x = particles[i].rect.x + SIZE; 
          particles[i].rect.y = particles[i].rect.y + SIZE; 
      } else if (pos.diag1 && r <= 100 && particles[i].rect.y <= 170) {
          particles[i].rect.x = particles[i].rect.x - SIZE; 
          particles[i].rect.y = particles[i].rect.y + SIZE; 
      }  else if (!pos.top) {

          particles[i].rect.x = particles[i].rect.x; 
          particles[i].rect.y -= 2; 

          particles[pos.top_index].rect.x = particles[pos.top_index].rect.x;
          particles[pos.top_index].rect.y += 2;

      } else {
          particles[i].rect.x = particles[i].rect.x; 
          particles[i].rect.y = particles[i].rect.y; 
      }

    }
  }
}

int main() {
  SDL_Window *window;
  SDL_Renderer *renderer;

  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow(
      "Particle Sim",
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
  SDL_PollEvent(&event);

  while (true) {

    for (int iter = 0; iter < NUM_OF_ITER; iter++) {

      SDL_PollEvent(&event);

      switch (event.type) {
        case SDL_QUIT:
          clear(renderer);
          SDL_quit(renderer, window);
          exit(1);
      }

      draw(renderer);
      SDL_RenderPresent(renderer);
      clear(renderer);
      update();
      delay(50);
    }

  clear(renderer);

  SDL_quit(renderer, window);

  return 0;

  }
}
