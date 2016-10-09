#include <SDL2/SDL.h>
#include <iostream>


int close(SDL_Window* gWindow);

int main()
{
	unsigned char key[16];	

	//Create Chip8 Object
	
	//Setup SDL2
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 320;
	SDL_Window* gWindow = NULL;
	//SDL_Surface* gScreenSurface = NULL;
	SDL_Renderer* gRenderer = NULL;
	//Event for keys pressed
	SDL_Event keysPressed;


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
	
	//chip8.load("rom_name");


	//Loop chip8.cycle() every second
	

	//store key presses by looping through current keys pressed and
	//sotring in chip8.keys[16] member and iterating the index by 1
	while(SDL_PollEvent(&keysPressed))
	{
		switch (keysPressed.type)
		{
			case SDLK_1:
				//set key[1] to 1 to indicate it's pressed
				key[1] = 1;
				break;
			case SDLK_2:
				key[2] = 1;
				break;
			case SDLK_3:
				key[3] = 1;
				break;
			case SDLK_4:
				key[0xC] = 1;
			case SDLK_q:
				key[4] = 1;
				break;
			case SDLK_w:
				key[5] = 1;
				break;
			case SDLK_e:
				key[6] = 1;
				break;
			case SDLK_r:
				key[0xD] = 1;
				break;
			case SDLK_a:
				key[7] = 1;
				break;
			case SDLK_s:
				key[8] = 1;
				break;
			case SDLK_d:
				key[9] = 1;
				break;
			case SDLK_f:
				key[0xE] = 1;
				break;
			case SDLK_z:
				key[0xA] = 1;
				break;
			case SDLK_x:
				key[0] = 1;
				break;
			case SDLK_c:
				key[0xB] = 1;
				break;
			case SDLK_v:
				key[0xF] = 1;
				break;
		}
	}	

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

