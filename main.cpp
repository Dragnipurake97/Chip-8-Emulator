#include <SDL2/SDL.h>
#include <iostream>


int close(SDL_Window* gWindow);

int main()
{
	//Create Chip8 Object
	
	//Setup SDL2
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 320;
	SDL_Window* gWindow = NULL;
	//SDL_Surface* gScreenSurface = NULL;
	SDL_Renderer* gRenderer = NULL;


	//Initialise SDL
	 if(SDL_Init( SDL_INIT_VIDEO ) < 0)
	{	
		std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		gWindow = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
				SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if( gWindow == NULL)
		{
			std::cout << "Window couldn't be created: " << SDL_GetError() << std::endl;
		}
		else
		{
			//gScreenSurface = SDL_GetWindowSurface( gWindow );
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				std::cout << "Renderer couldn't be created: " << SDL_GetError() << std::endl;
			}
		}
	}
	
	SDL_Delay(3000);

	//Example pixel
	SDL_Rect px = {10, 10, 10, 10};
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(gRenderer, &px);

	SDL_UpdateWindowSurface( gWindow );
	SDL_RenderPresent( gRenderer );

	//Load ROM
	
	//Loop chip8.cycle() every second
	

	//store key presses
	

	//update graphics if drawflag is true
	/*
	if(drawFlag == true)
	{
		SDL_UpdateWindowSurface( gWindow );
		SDL_RenderPresent( gRenderer );
	}
	*/
	
	SDL_Delay(3000);
	
	//Shutdown SDL
	close(gWindow);
	return 0;
}


//Close SDL function
int close(SDL_Window* gWindow)
{
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
	return 0;
}

