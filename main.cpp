#include <SDL2/SDL.h>
#include <iostream>
#include "chip8.h"

int close(SDL_Window* gWindow);
void getKeys(SDL_Event keysPressed, Chip8 chip8);

int main()
{	
	bool drawFlag = true;
	//Create Chip8 Object
	Chip8 chip8;
	//Setup SDL2
	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 320;
	SDL_Window* gWindow = NULL;
	//SDL_Surface* gScreenSurface = NULL;
	SDL_Renderer* gRenderer = NULL;
	//Event for keys pressed
	SDL_Event keysPressed;
	//Pixel coordinates
	int p_x;
	int p_y;	



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
	
	//Create black background
	//SDL_Rect background = {0, 0, 320, 640};
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	//SDL_RenderFillRect(gRenderer, &background);
	

	//Create pixel
	SDL_Rect px;

	//Update surface with background
	SDL_UpdateWindowSurface( gWindow );
	SDL_RenderPresent( gRenderer );


	SDL_Delay(3000);


	//Load ROM	
	//chip8.load("rom_name");

	


	//Loop
	while(true)
	{

		//clear keys
		chip8.clearKeys();

		//store current keys pressed
		getKeys(keysPressed, chip8);
		
		//Cycle 1 opcode
		chip8.cycle();

		chip8.updateScreen(gRenderer, px);
		SDL_UpdateWindowSurface(gWindow);
		SDL_RenderPresent(gRenderer);	
	

		//Delay 1sec
		SDL_Delay(1000);
	}

	//Shutdown SDL
	close(gWindow);
	return 0;
}


void getKeys(SDL_Event keysPressed, Chip8 chip8)
{	
	while(SDL_PollEvent(&keysPressed))
	{
		switch (keysPressed.type)
		{
			case SDLK_1:
				//set key[1] to 1 to indicate it's pressed
				chip8.key[1] = 1;
				break;
			case SDLK_2:
				chip8.key[2] = 1;
				break;
			case SDLK_3:
				chip8.key[3] = 1;
				break;
			case SDLK_4:
				chip8.key[0xC] = 1;
			case SDLK_q:
				chip8.key[4] = 1;
				break;
			case SDLK_w:
				chip8.key[5] = 1;
				break;
			case SDLK_e:
				chip8.key[6] = 1;
				break;
			case SDLK_r:
				chip8.key[0xD] = 1;
				break;
			case SDLK_a:
				chip8.key[7] = 1;
				break;
			case SDLK_s:
				chip8.key[8] = 1;
				break;
			case SDLK_d:
				chip8.key[9] = 1;
				break;
			case SDLK_f:
				chip8.key[0xE] = 1;
				break;
			case SDLK_z:
				chip8.key[0xA] = 1;
				break;
			case SDLK_x:
				chip8.key[0] = 1;
				break;
			case SDLK_c:
				chip8.key[0xB] = 1;
				break;
			case SDLK_v:
				chip8.key[0xF] = 1;
				break;
		}
	}	
}


//Close SDL function
int close(SDL_Window* gWindow)
{
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
	return 0;
}

