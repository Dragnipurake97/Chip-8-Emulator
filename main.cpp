#include <SDL.h>
#include <iostream>
#include "chip8.h"

int close(SDL_Window* gWindow);
void getKeys(Chip8 chip8, SDL_Event keysPressed);

int main(int argc, char **argv)
{	
	bool drawFlag = false;
	//Create Chip8 Object
	Chip8 chip8;
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
	
	//Create black background
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(gRenderer);
	//SDL_RenderFillRect(gRenderer, &background);
	

	//Update surface with background
	SDL_UpdateWindowSurface( gWindow );
	SDL_RenderPresent( gRenderer );


	SDL_Delay(3000);


	//Load ROM	
	chip8.load("MISSILE");

	
	SDL_Event keysPressed;
	

	//Take snapshot of keyboard 
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	//Loop
	while(true)
	{

		//clear keys
		chip8.clearKeys();

		//Get keys
		//Update keyboard values
		SDL_PumpEvents();
		//Must use if so each value gets tested
		if (currentKeyStates[SDL_SCANCODE_1])
		{
			//set key[1] to 1 to indicate it's pressed
			chip8.key[1] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_2])
		{
			chip8.key[2] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_3])
		{
			chip8.key[3] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_4])
		{
			chip8.key[0xC] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_Q])
		{
			chip8.key[4] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_W])
		{
			chip8.key[5] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_E])
		{
			chip8.key[6] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_R])
		{
			chip8.key[0xD] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_A])
		{
			chip8.key[7] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_S])
		{
			chip8.key[8] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_D])
		{
			chip8.key[9] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_F])
		{
			chip8.key[0xE] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_Z])
		{
			chip8.key[0xA] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_X])
		{
			chip8.key[0] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_C])
		{
			chip8.key[0xB] = 1;
		}
		if (currentKeyStates[SDL_SCANCODE_V])
		{
			chip8.key[0xF] = 1;
		}
		//debug help
		if (currentKeyStates[SDL_SCANCODE_P])
		{
			chip8.dumpRegisters();
		}

		
		//Start cycle for next opcode
		drawFlag = chip8.cycle();

		//Draw to screen if sprite was drawn
		if (drawFlag == true)
		{
			std::cout << "Draw" << std::endl;
			chip8.updateScreen(gRenderer);
			SDL_UpdateWindowSurface(gWindow);
			SDL_RenderPresent(gRenderer);
		}

		//Delay by 16ms (1000/60) so 60 instructions second
		SDL_Delay(16);
	}

	//Shutdown SDL
	close(gWindow);
	return 0;
}


void getKeys(Chip8 chip8)
{	
	SDL_Event keysPressed;
	while(SDL_PollEvent(&keysPressed) != 0)
	{
		switch (keysPressed.type)
		{
			case SDLK_1:
				//set key[1] to 1 to indicate it's pressed
				chip8.key[1] = 1;
				std::cout << "1" << std::endl;
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

