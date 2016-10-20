#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <time.h>
#include "chip8.h"



Chip8::Chip8()
{
	//fontset
	unsigned char fontset[69] = 
					{0x30, 0x39, 0x22, 0x2A, 0x3E, 				     
				     0x20, 0x24, 0x34, 0x26, 0x28, 
				     0x2E, 0x18, 0x14, 0x1C, 0x10, 
				     0x12,                               //Lookup table
				     0xF0, 0x80, 0xF0, 0x80, 0xF0,
       				 0x80, 0x80, 0x80, 0xF0, 0x50,
				     0x70, 0x50, 0xF0, 0x50, 0x50,
				     0x50, 0xF0, 0x80, 0xF0, 0x10,
				     0xF0, 0x80, 0xF0, 0x90, 0xF0,
				     0x90, 0xF0, 0x90, 0xF0, 0x10,
				     0xF0, 0x10, 0xF0, 0x90, 0xF0,
				     0x90, 0x90, 0x90, 0xF0, 0x10,
				     0x10, 0x10, 0x10, 0x60, 0x20,
				     0x20, 0x20, 0x70, 0xA0, 0xA0,
				     0xF0, 0x20, 0x20};

	//Set all the various registers and such to default value
	pc = 0x200;
	opcode = 0x00;
	I = 0x00;
	sp = 0x00;
	delay_timer = 0x00;
	sound_timer = 0x00;
	drawFlag = false;
	
	
	//Reset registers, stack and keys
	for(int i = 0; i < 16; i++)
	{
		V[i] = 0;
		stack[i] = 0;
		key[i] = 0;
	}

	//clear screen
	for(int i = 0; i < 2048; i++)
	{
		screen[i] = 0;
	}

	//Load fontset
	for(int i = 0; i < 69; i++)
	{
		memory[i] = fontset[i];
	}
	std::cout << "Chip 8 initialized" << std::endl;
}

void Chip8::load(std::string rom_name)
	{
		std::streampos size;
		//std::ios::ate sets position to end of file so we can get the size
		std::ifstream rom(rom_name.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
		if(rom.is_open())
		{	
			//Create array to take in binary file data
			char * mblock;
			std::cout << "rom opened" << std::endl;
			size = rom.tellg();
			mblock = new char [size];
			//Set position back to start of file
			rom.seekg(0, std::ios::beg);
			//Read memory into array called mblock
			rom.read(mblock, size);
			//close file
			rom.close();
			std::cout << "rom closed" << std::endl;

			//Transfer memory from mblock to memory
			for (int i = 0; i < size; i++)
			{
				//start at 0x200 as below that is reserved
				memory[0x200 + i] = mblock[i];
			}
			std::cout << "Rom loaded" << std::endl;
		}

	}

bool Chip8::cycle()
{
	//Fetch
	//Build opcode by combining 2 places in memory to form 
	//a 2 byte opcode, then incrment pc by 2 as used 2 places in memory
	opcode = (memory[pc] << 8) | memory[pc + 1];
	
	//Decode & Execute
	switch(opcode & 0xF000)
	{
		//Special case for opcodes beginning with 0x0
		case 0x0000:
			switch(opcode & 0x000F)
			{
				case 0x0000:
					//Clear Screen
					for(int i = 0; i < 2048; i++)
					{
						screen[i] = 0;
					}
					pc += 2;
					break;
				case 0x000E:
					//Load pc from stack to RTS
					pc = stack[sp--];
					break;
			}
			break;
		case 0x1000:
			//Jump
			pc = (opcode & 0x0FFF);
			break;
		case 0x2000:
			//Jump subroutine
			stack[sp] = pc;
			sp++;
			pc = (opcode & 0x0FFF);
			break;
		case 0x3000:
			//Get register number, shit right by 8 to leave with 0x0F
			if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
			{	
				//increment pc by 4 to skip next instruction
				pc+=4;
			}
			else
			{
				pc += 2;
			}
			break;
		case 0x4000:
			if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x0FF))
			{
				pc+=4;
			}
			else
			{
				pc += 2;
			}
			break;
		case 0x5000:
			if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
			{
				pc+= 4;
			}
			else
			{
				pc += 2;
			}
			break;
		case 0x6000:
			V[(opcode & 0x0F00) >> 8] = opcode & 0xFF;
			pc += 2;
			break;
		case 0x7000:
			V[(opcode & 0x0F00) >> 8] += opcode & 0x0FF;
			pc += 2;
			break;
		case 0x9000:
			if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
			{
				pc += 4;
			}
			else
			{
				pc += 2;
			}
			break;
		case 0xA000:
			I = opcode & 0x0FFF;
			pc += 2;
			break;
		case 0xB000:
			pc = (opcode & 0x0FFF) + V[0];
			break;
		case 0xC000:
			srand(time(NULL));
			V[(opcode & 0x0F00) >> 8] = (0x00FF & (std::rand() % 0xFF));
			pc += 2;
			break;
		case 0xD000:
			unsigned char x = V[(opcode & 0x0F00) >> 8];
			unsigned char y = V[(opcode & 0x00F0) >> 4];
			unsigned char height = opcode & 0x000F;
			unsigned short pixel;
			//set carry register to 0
			V[15] = 0;
			for (int yline = 0; yline < height; yline++)
			{
				//Get pixels value for this x axis line 
				pixel = memory[I + yline];
				//loop through row and set pixels
				for(int xbit = 0; xbit < 8; xbit++)
				{
					if((pixel & (0x80 >> xbit)) != 0)
					{
						//get right pixel by adding binary position to x coord and times y one by 64
						if(screen[(x + xbit + ((y + yline) * 64))] == 1)
						{
							//set flag to 1 if collision
							V[15] = 1;
						}
						//NOR pixel so if alreay 1 goes to 0 and if 0 goes to 1
						screen[x + xbit + ((y + yline) * 64)] ^= 1;
					}
				}
			}
			//DRAW GRAPHICS
			drawFlag = true;
			pc += 2;
			break;
		
				

	}		
		
	switch(opcode & 0xF00F)
	{
		case 0x8000:
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x8001:
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x8002:
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x8003:
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x8004:
			if((V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]) > 255)
			{
				V[15] = 1;
				//V[vx_index] += V[vy_index];
			}
			else
			{
				V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F) >> 4];
				V[15] = 0;
			}
			pc += 2;
			break;	
		case 0x8005:
			if((V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4]) < 0)
			{
				V[15] = 1;
				//V[vx_index] -= V[vy_index];
			}
			else
			{
				V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
				V[15] = 0;
			}
			pc += 2;
			break;
		case 0x8006:
			V[15] = ((opcode & 0x0F00) >> 8) & (0x80 >> 7);
			V[(opcode & 0x0F00) >> 8] >> 4;
			pc += 2;
			break;
		case 0x8007:
			if((V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8]) < 0)
			{
				V[15] = 0;
				//V[vy_index] -= V[vx_index];
			}
			else
			{
				V[15] = 1;
				V[(opcode & 0x00F0) >> 4] -= V[(opcode & 0x0F00) >> 8];
			}
			pc += 2;
			break;
		case 0x800E:
			V[15] = V[(opcode & 0x0F00) >> 8] & 0x80;
			V[(opcode & 0x0F00) >> 8] << 4;
			pc += 2;
			break;
	}

	switch(opcode & 0xF0FF)
	{
		case 0xE09E:
			//check if key stored in V[X] is 1 (active)
			if(key[V[(opcode & 0x0F00)] >> 8] == 1)
			{
				pc+=4;
			}
			else
			{
				pc += 2;
			}
			break;
		case 0xE0A1:
			//If the key isn't 1 (pressed) then skip
			if(key[V[(opcode & 0x0F00) >> 8]] != 1)
			{
				pc += 4;
			}
			else
			{
				pc += 2;
			}
			break;
		case 0xF007:
			V[(opcode & 0x0F00) >> 8] = delay_timer;
			pc += 2;
			break;
		case 0xF00A:
			V[(opcode & 0x0F00) >> 8] = key[0];
			pc += 2;
			break;
		case 0xF015:
			delay_timer = V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0xF018:
			sound_timer = V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0xF01E:
			I += V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x0F029:
			//Font table is from 0x0000 - 0x0010 so the hex character alone will point to its alternative address 
			//Going to store fonts from address 0x0010 - 0x003E so add lookup address to that
			I = 0x0010 + memory[V[(opcode & 0x0F00) >> 8] & 0x000F];
			pc += 2;
			break;
		case 0xF033:
			memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
			memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 100;
			memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
			pc += 2;
			break;
		case 0xF055:
			for(int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
			{
				memory[I + i] = V[i];
			}
			pc += 2;
			break;
		case 0xF065:
			for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
			{	
				V[i] = memory[I + i];			
			}
			pc += 2;
			break;
	}	
	

	//Update Timers
	//If timers are over 0 then decrement by 1 each cycle
	if(delay_timer > 0)
	{
		delay_timer--;
	}

	if(sound_timer > 0)
	{
	sound_timer--;
	}
	else
	{
		//Beep
		std::cout << "Beep" << std::endl;
	}

	return drawFlag;

}

void Chip8::updateScreen(SDL_Renderer* gRenderer)
{
	//Create pixel
	SDL_Rect px;

	//Clear current pixels
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(gRenderer);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	for(int p = 0; p < (32 * 64); p++)
	{
		if(screen[p] == 1)
		{
			//x is %64 as remainder would be x coord and y is /64 as each ste of 64 elements is 1 line of x pixels
			px = {((p % 64) * 10), ((p / 64) * 10), 10, 10};
			SDL_RenderFillRect(gRenderer, &px);
		}			
	}
	

	//clear draw flag
	drawFlag = false;
}


bool Chip8::returnDraw()
{
	return drawFlag;
}

void Chip8::dumpRegisters()
{
	std::cout << "Registers: " << std::endl;
	for(int i = 0; i < 16; i++)
	{
		std::cout << std::hex << "Register " << i << " = " << static_cast<unsigned>(V[i]) << std::endl;
	}
	std::cout << std::hex << "Opcode = " << opcode << std::endl;
	std::cout << std::hex << "Program Counter = " << pc << std::endl;
	std::cout << std::hex << "Index Register = " << I << std::endl;
	std::cout << "Stack: " << std::endl;
	for(int i = 0; i < 16; i++)
	{
		std::cout << "Stack level " << i << " = " << stack[i] << std::endl;
	}
	std::cout << std::hex << "Stack Pointer = " << sp << std::endl;
	std::cout << std::hex << "Delay Timer = " << delay_timer << std::endl;
	std::cout << std::hex << "Sound Timer = " << sound_timer << std::endl;
	std::cout << "Keys: " << std::endl;
	for(int i = 0; i < 16; i++)
	{
		std::cout << std::hex << "Key " << i << " = " << static_cast<unsigned>(key[i]) << std::endl;
	}
}

void Chip8::clearKeys()
{
	for(int i = 0; i < 16; i++)
	{
		key[i] = 0x00;
	}	
}

