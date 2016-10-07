#include <iostream>


class Chip8()
{
	//16 registers (each holding 2 bytes named VX
	unsigned char V[16];
	//4k memory
	unsigned char memory[4096];
	//current opcode
	unsigned short opcode;
	//Program Counter and index register
	unsigned short pc;
	unsigned short I;
	//Stack and stack pointer
	unsigned short stack[16];
	unsigned short sp;
	//Screen pixels stored in an array (0 = off, 1 = on)
	unsigned char screen[64 * 32];
	//timers
	unsigned char delay_timer;
	unsigned char sound_timer;
	//Keypad
	unsigned char key[16];
	//fontset
	unsigned char fontset[80];


	Chip8::Chip8(void)
	{
	//Set all the various registers and such to default value
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;
	delay_timer = 0;
	sound_timer = 0;
	
	//Reset registers and stack
	for(int i = 0; i < 16; i++)
	{
		V[i] = 0;
		stack[i] = 0;
	}

	//clear screen
	for(int i = 0; i < 2048; i++)
	{
		screen[i] = 0;
	}

	//Load fontset
	for(int i = 0; i < 80; i++)
	{
		memory[i] = fontset[i];
	}

	//Load ROM
	

	}


	chip8::cycle()
	{
		//Fetch
		//Build opcode by combining 2 places in memory to form 
		//a 2 byte opcode, then incrment pc by 2 as used 2 places in memory
		opcode = (memory[pc] << 8) | memory[pc + 1];
		pc += 2;

		//Decode & Execute
		switch(opcode & 0xF000)
		{
			//Special case for opcodes beginning with 0x0
			case 0x0000:
				switch(opcode & 0x000F)
				{
					case 0x0000:
						//Clear Screen
						for(int i = 0; i < 2048)
						{
							screen[i] = 0;
						}
						break;
					case 0x000E:
						//Load pc from stack to RTS
						pc = stack[sp];
						break;
				}
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
				//Get register number, divide by 256 to get single number
				unsigned char vx_index = (opcode & 0x0F00) / 256;
				if(V[vx_index] == (opcode & 0x00FF)
				{	
					//increment pc by 2 to skip next instruction
					pc+=2;
				}
				break;
			case 0x4000:
				unsigned char vx_index = (opcode & 0x0F00) / 256;
				if(V[vx_index] != (opcode & 0x0FF))
				{
					pc+=2;
				}
				break;
			case 0x5000:
				unsigned char vx_index = (opcode & 0x0F00) / 256;
				unsigned char vy_index = (opcode & 0x00F0) / 16;
				if(V[vx_index] == V[vy_index])
				{
					pc+= 2;
				}
				break;
			case 0x6000:
				unsigned char vx_index = (opcode & 0x0F00) / 256;
				V[vx_index] = opcode & 0xFF;
				break;
			case 0x7000:
				unsigned char vx_index = (opcode & 0x0F00) / 256;
				V[vx_index] += opcode & 0x0FF;
				break;
			case 0x9000:
				vx_index = (opcode & 0x0F00) / 256;
				vy_index = (opcode & 0x00F0) / 16;
				if(V[vx_index] != V[vy_index])
				{
					pc += 2;
				}
				break;
			case 0xA000:
				I = 0x0FFF;
				break;
			case 0xB000:
				pc = (opcode & 0x0FFF) + V[0];
				break;
			case 0xC000:
				vx_index = (opcode & 0x0F00) / 256;
				V[vx_index] = 0x00FF & std::rand();
				break;
			case 0xD000:
				vx_index = (opcode & 0x0F00) / 256;
				vy_index = (opcode & 0x00F0) / 16;
				unsigned char x = V[vx_index];
				unsigned char y = V[vy_index];
				unsigned char height = opcode & 0xF;
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
							screen[x + xbit + ((y + yline) * 64)] ^= 1'
						}
					}
				}
				//DRAW GRAPHICS
				break;
		
				

		}		
		
		switch(opcode & 0xF00F)
		{
			case 0x8000:
				vx_index = (opcode & 0x0F00) / 256;
				vy_index = (opcode & 0x00F0) / 16;
				V[vx_index] = V[vy_index];
				break;
			case 0x8001:
		 		vx_index = (opcode & 0x0F00) / 256;
				vy_index = (opcode & 0x00F0) / 16;
				V[vx_index] = V[vx_index] | V[vy_index];
				break;
			case 0x8002:
				vx_index = (opcode & 0x0F00) / 256;
				vy_index = (opcode & 0x00F0) / 16;
				V[vx_index] = V[vx_index] & V[vy_index];
				break;
			case 0x8003:
				vx_index = (opcode & 0x0F00) / 256;
				vy_index = (opcode & 0x00F0) / 16;
				V[vx_index] = V[vx_index] ^ V[vy_index];
				break;
			case 0x8004:
				vx_index = (opcode & 0x0F00) / 256;
				vy_index = (opcode & 0x00F0) / 16;
				if((V[vx_index] + V[vy_index]) > 255)
				{
					V[15] = 1;
					//V[vx_index] += V[vy_index];
				}
				else
				{
					V[vx_index] += V[vy_index];
					V[15] = 0;
				}
				break;	
			case 0x8005:
				vx_index = (opcode & 0x0F00) / 256;
				vy_index = (opcode & 0x00F0) / 16;
				if((V[vx_index] - V[vy_index]) < 0)
				{
					V[15] = 1;
					//V[vx_index] -= V[vy_index];
				}
				else
				{
					V[vx_index] -= V[vy_index];
					V[15] = 0;
				}
				break;
			case 0x8006:
				vx_index = (opcode & 0x0F00);
				V[15] = vx_index & (0x80 >> 7);
				V[vx_index] >> 1;
				break;
			case 0x8007:
				vx_index = (opcode & 0x0F00) / 256;
				vy_index = (opcode & 0x00F0) / 16;
				if((V[vy_index] - V[vx_index]) < 0)
				{
					V[15] = 0;
					//V[vy_index] -= V[vx_index];
				}
				else
				{
					V[15] = 1;
					V[vy_index] -= V[vx_index];
				}
				break;
			case 0x800E:
				vx_index = (opcode & 0x0F00) / 256;
				V[F] = 	V[vx_index] & 0x80;
				V[vx_index] << 1;
				break;
								
		}

		switch(opcode & 0xF0FF)
		{
			case 0xE09E:
				vx_index = (opcode & 0x0F00) / 256;
				if(key == V[vx_index])
				{
					pc+=2;
				}
				break;
			case 0xE0A1:
				vx_index = (opcode & 0x0F00) / 256;
				if( key != V[vx_index])
				{
					pc += 2;
				}
				break;
			case 0xF007:
				vx_index = (opcode & 0x0F00) / 256;
				V[vx_index] = delay_timer;
				break;
			case 0xF00A:
				vx_index = (opcode & 0x0F00) / 256;
				V[X] = key;
				break;
			case 0xF015:
				vx_index = (opcode & 0x0F00) / 256;
				delay_timer = V[vx_index];
				break;
			case 0xF018:
				vx_index = (opcode & 0x0F00) / 256;
				sound_timer = V[vx_index];
				break;
			case 0xF01E:
				vx_index = (opcode & 0x0F00) / 256;
				I += V[vx_index];
				break
			case 0x0F029:
				vx_index = (opcode & 0x0F00) / 256;
				//Font table is from 0x0000 - 0x0010 so the hex character alone will point to its alternative address 
				//Going to store fonts from address 0x0010 - 0x003E so add lookup address to that
				I = 0x0010 + memory[V[vx_index] & 0x000F];
				break;
			case 0xF033:
				vx_index = (opcode & 0x0F00) / 256;
				memory[I] = V[vx_index] / 100;
				memory[I + 1] = (V[vx_index] / 10) % 100;
				memory[I + 2] = (V[vx_index] % 100) % 10;
				break;
			case 0xF055:
				vx_index = (opcode & 0x0F00) / 256; // or >> 8
				for(int i = 0; i <= vx_index; i++)
				{
					memory[I + i] = V[i];
				}
				break;
			case 0xF065:
				vx_index = (opcode & 0x0F00) / 256;
				for (int i = 0; i <= vx_index; i++)
				{	
					V[i] = memory[I + i];			
				}
				break;

		}	
	

		//Update Timers
		//If timers are over 0 then decrement by 1 each cycle
		if(delay_timer > 0){
			delay_timer--;
		}

		if(sound_timer > 0){
			if(sound_timer == 0){
				//Add SDL Beep
				std::cout << "Beep" << std::endl;
			}
		sound_timer--;
		}

	}
}



int main() {

	return 0;

}
