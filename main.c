#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_render.h"

#define WIDTH 800.0f
#define HEIGHT 600.0f
#define NUM_OF_PARTICLES 600
#define SIZE 2 

typedef struct {
  SDL_Rect rect;
} particle;

particle particles[NUM_OF_PARTICLES];

void populate(int num_of_particles) {
  for (int i = 0; i < num_of_particles; i++) {
    particle temp;
    temp.rect.x = 100 + i * SIZE;
    temp.rect.y = 100;
    particles[i] = temp;
  }
}

int main()
{
  bool quit = false;

  SDL_Event event;

  // init SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window * window = SDL_CreateWindow("SDL2 line drawing",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);


  particle part1;
  particle part2;
  part1.rect.x = 250;
  part1.rect.y = 150;
  part1.rect.w = SIZE;
  part1.rect.h = SIZE;

  part2.rect.x = 250;
  part2.rect.y = 150 - SIZE;
  part2.rect.w = SIZE;
  part2.rect.h = SIZE;

        
  populate(NUM_OF_PARTICLES);
  // handle events
  while (!quit)
  {
      SDL_Delay(10);
      SDL_PollEvent(&event);

      switch (event.type)
      {
          case SDL_QUIT:
              quit = true;
              break;
      }

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);

      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

      /*SDL_RenderDrawLine(renderer, 100, 100, 500, 500);*/

      for (int i = 0; i < NUM_OF_PARTICLES; i++) {
        SDL_RenderDrawRect(renderer, &particles[i].rect);
      }
      SDL_RenderDrawRect(renderer, &part1.rect);
      SDL_RenderDrawRect(renderer, &part2.rect);

      /*for (int i = 0; i < 100; i++) {*/
        /*SDL_RenderDrawPoint(renderer, 150 + i, 150);*/
        /*sleep(1);*/
        /*SDL_RenderPresent(renderer);*/
      /*}*/

      // render window
      SDL_RenderPresent(renderer);
  }

  (void) part1;

  // cleanup 
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
