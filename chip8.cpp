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
					{0x32, 0x3B, 0x24, 0x2C, 0x41, 				     
				     0x22, 0x26, 0x36, 0x28, 0x2A, 
				     0x31, 0x1A, 0x16, 0x1E, 0x12, 
				     0x14,                               //Lookup table
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
			size = rom.tellg();
			mblock = new char [size];
			//Set position back to start of file
			rom.seekg(0, std::ios::beg);
			//Read memory into array called mblock
			rom.read(mblock, size);
			//close file
			rom.close();

			//Transfer memory from mblock to memory
			for (int i = 0; i < size; i++)
			{
				//start at 0x200 as below that is reserved
				memory[0x200 + i] = mblock[i];
			}
			std::cout << "ROM LOADED" << std::endl;
		}

	}

bool Chip8::cycle()
{
	//Fetch
	//Build opcode by combining 2 places in memory to form 
	//a 2 byte opcode, then incrment pc by 2 as used 2 places in memory
	opcode = (memory[pc] << 8) | memory[pc + 1];
	
	//For debugging
	std::cout << std::hex << opcode << std::endl;

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
					pc = stack[sp];
					sp--;
					pc += 2;
					break;
			}
			break;
		case 0x1000:
			//Jump
			pc = (opcode & 0x0FFF);
			break;
		case 0x2000:
			//Jump subroutine
			sp++;
			stack[sp] = pc;
			pc = (opcode & 0x0FFF);
			break;
		case 0x3000:
			//Skips the next instruction if VX equals NN
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
			//Skips the next instruction if VX doesn't equal NN.
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
			//Skips the next instruction if VX equals VY.
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
			//	Sets VX to NN.
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			pc += 2;
			break;
		case 0x7000:
			//Adds NN to VX.
			V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
			pc += 2;
			break;
		case 0x9000:
			//	Skips the next instruction if VX doesn't equal VY.
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
			//	Sets I to the address NNN.
			I = opcode & 0x0FFF;
			pc += 2;
			break;
		case 0xB000:
			//Jumps to the address NNN plus V0.
			pc = (opcode & 0x0FFF) + V[0];
			break;
		case 0xC000:
			//Sets VX to the result of a bitwise and operation on a random number and NN.
			srand(time(NULL));
			V[(opcode & 0x0F00) >> 8] = ((opcode &0x00FF) & (std::rand() % 0xFF));
			pc += 2;
			break;
		case 0xD000:
			//Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I;
			//I value doesn’t change after the execution of this instruction. As described above, 
			//VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
			//http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/ - Taken from here
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
			//Sets VX to the value of VY.
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x8001:
			//Sets VX to VX or VY.
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x8002:
			//Sets VX to VX and VY.
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x8003:
			//Sets VX to VX xor VY.
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x8004:
			//Adds VY to VX.VF is set to 1 when there's a carry, and to 0 when there isn't.
			if((V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]) > 255)
			{
				V[15] = 1;
			}
			else
			{
				V[15] = 0;
			}
			V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F) >> 4];
			pc += 2;
			break;	
		case 0x8005:
			//	VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			if((V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4]))
			{
				V[15] = 1;
			}
			else
			{
				V[15] = 0;
			}
			V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x8006:
			//Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
			V[15] = (V[(opcode & 0x0F00)] & -V[(opcode & 0x0F00)]);
			V[(opcode & 0x0F00) >> 8] >>= 1;
			pc += 2;
			break;
		case 0x8007:
			//Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			if((V[(opcode & 0x00F0) >> 4] < V[(opcode & 0x0F00) >> 8]))
			{
				V[15] = 0;
			}
			else
			{
				V[15] = 1;
			}
			V[(opcode & 0x0F00) >> 4] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
		case 0x800E:
			//Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
			V[15] = V[(opcode & 0x0F00) >> 8] >> 7;
			V[(opcode & 0x0F00) >> 8] <<= 1;
			pc += 2;
			break;
	}

	switch(opcode & 0xF0FF)
	{
		case 0xE09E:
			//Skips the next instruction if the key stored in VX is pressed.
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
			//Skips the next instruction if the key stored in VX isn't pressed.
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
			//	Sets VX to the value of the delay timer.
			V[(opcode & 0x0F00) >> 8] = delay_timer;
			pc += 2;
			break;
		case 0xF00A:
			//A key press is awaited, and then stored in VX.
			//Check for pressed key
			for (unsigned char i = 0x00; i < 16; i++)
			{
				if (key[i] == 1)
				{
					V[(opcode & 0x0F00) >> 8] = i;
					//Only increment if key was pressed, else will repeat cycle
					pc += 2;
					break;
				}
			}
			break;
		case 0xF015:
			//	Sets the delay timer to VX.
			delay_timer = V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0xF018:
			//Sets the sound timer to VX.
			sound_timer = V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0xF01E:
			// Adds VX to I.
			I += V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x0F029:
			//Sets I to the location of the sprite for the character in VX.Characters 0 - F(in hexadecimal) are represented by a 4x5 font.
			//Font lookup table is from 0x0000 - 0x0010 so the hex character alone will point to its alternative address 
			//Going to store fonts from address 0x0010 - 0x003E so add lookup address to that
			I = memory[V[(opcode & 0x0F00) >> 8] & 0xF];
			pc += 2;
			break;
		case 0xF033:
			//Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1,
			//and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, 
			//place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)
			memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
			memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 100;
			memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
			pc += 2;
			break;
		case 0xF055:
			//Stores V0 to VX (including VX) in memory starting at address I.
			for(int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
			{
				memory[I + i] = V[i];
			}
			pc += 2;
			break;
		case 0xF065:
			//Fills V0 to VX (including VX) with values from memory starting at address I.
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
		if (sound_timer == 0)
		{
			//beep
		}
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

