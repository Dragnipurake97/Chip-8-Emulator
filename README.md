# Chip-8-Emulator
A CHIP-8 emulator written in C++ to teach myself how to program a basic emulator. This will use SDL2 to handle graphics.

## To Do
1. Shift all functions into chip8.cpp so all variables are private
2. Add SDL beeps for when sound_timer == 0
3. Add exception handling:
    * ROM too big
    * Uncompatible memory types
4. Add crash handling


## Components
### Registers, Memory and Timers
|Component|Description|Implementation|
|---------|-----------|--------------|
|Registers|There are 16 registers V0 - V15|Unsigned char V[16]|
|Stack|Stack to store return address|Unsigned short stack[16]|
|Stack Pointer|Indicates level of stack|Unsigned short sp|
|Program Counter|Current place in memory|Unsigned short pc|
|Memory|4k bytes of memory in for 0xFF|Unsigned char memory[4096]|
|Index Register|Used to store useful instruction info|Unsigned short I|
|Screen|Black or white screen of 64x32p|Unsigned char screen[64 * 32]|
|Delay Timer|Timer used for time game events|Unsigned char delay_timer|
|Sound Timer|Timer used for sfx !0 = beeping|Unsigned char sound_timer|
|Keypad|Hex keypad 0x0 - 0xF, 1 = pressed|Unsigned char keypad[16]|

### Font-Table
Storing the font lookup table from 0x0000 - 0x0010 (memory[0] - memory[0xF]) and points to the value in the font table where the start of the hex number to be printed is. The hex number to be printed is stored in its corrosponding place in memory e.g. 0xA is stored in memory[0xA].

|Hex Number| Memory Value|
|----------|-------------|
|0|30|
|1|39|
|2|22| 
|3|2A| 
|4|3E| 
|5|20| 
|6|24| 
|7|34| 
|8|26| 
|9|28| 
|A|2E| 
|B|18|
|C|14| 
|D|1C|
|E|10|
|F|12|

### Core Methods
**Initialise**:
> This method is a constructor and loads the font table into memory, resets all of the registers, stack and such.

**Load ROM**:
> Reads a binary file and loads it into memory.

**Cycle**:
> This is the main loop of the emulator and deals with the fetch>decode>execute cycle. This method should occur 60 times a second.

**getKeys**:
> This clears the keys currently stored and then gets the current keys pressed and stores them.

**dumpReigsters**:
> Prints all of the values for registers and such to help with debuging.


Resources:
http://lazyfoo.net/tutorials/SDL/index.php 
http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
