#include<SDL.h>
#include <string>
#include "simulate.h"



int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Projekt Winda", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);



	bool quit = false;

	SDL_Event event;

	
	Building building;

	while (!quit)
	{

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		SDL_Event e;

		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}

			

			

			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);

				building.Click(mouseX,mouseY);

			}

		
			
		}

		
		building.Update(10);
		building.Draw(renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(10);
		

		

	}

	

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	

	return 0;
}