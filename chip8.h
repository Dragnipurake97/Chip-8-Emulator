#ifndef CHIP8_H
#define CHIP8_H

#include <string>
#include <SDL2/SDL.h>

class Chip8
{
	private:
		unsigned char V[16];
		unsigned char memory[4096];
		unsigned short opcode;
		unsigned short pc;
		unsigned short I;
		unsigned short stack[16];
		unsigned char sp;
		unsigned char screen[64 * 32];
		unsigned char delay_timer;
		unsigned char sound_timer;
		//unsigned char key[16];
		bool drawFlag;
	public:
		unsigned char key[16];
		Chip8();
		void load(std::string rom_name);
		void cycle();
		void updateScreen(SDL_Renderer* gRenderer, SDL_Rect px);
		bool returnDraw();
		void dumpRegisters();
		void clearKeys();

};

#endif
