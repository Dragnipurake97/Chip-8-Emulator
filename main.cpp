#include <SDL.h>
#include <iostream>
#include "chip8.h"
#include <string>
#include <cstdlib>

//Prototypes
int close(SDL_Window* gWindow);

int main(int argc, char **argv)
{	
	// Draw flag for writing to screen
	bool drawFlag = false;

	//Get ROM name
	std::string rom_name;
	std::cout << "ROM name: ";
	std::cin >> rom_name;
	// If input fails
	if (std::cin.fail())
	{
		std::cout << "\n\nInvalid Name\n\n" << std::endl;
		return 0;
	}
	std::cout << std::endl;

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
	

	//Update surface with background
	SDL_UpdateWindowSurface( gWindow );
	SDL_RenderPresent( gRenderer );


	//Load ROM
	chip8.load(rom_name);

	//Create event for processing key presses
	SDL_Event keysPressed;
	

	//Take snapshot of keyboard 
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	//Loop
	while(true)
	{

		//clear keys
		chip8.clearKeys();

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
		// Exit
		if (currentKeyStates[SDL_SCANCODE_ESCAPE])
		{
			exit(0);
		}

		
		//Start cycle for next opcode
		drawFlag = chip8.cycle();

		//Draw to screen if sprite was drawn
		if (drawFlag == true)
		{
			chip8.updateScreen(gRenderer);
			SDL_UpdateWindowSurface(gWindow);
			SDL_RenderPresent(gRenderer);
		}

		
		//Debug delay
		//SDL_Delay(40);



		//Delay for 60hz roughly takes 2ms for a cycle but not perfect
		SDL_Delay(10);
	}

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

