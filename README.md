# Chip-8-Emulator
A CHIP-8 emulator written in C++ using SDL to handle graphical output.

## Input
Using keyboard control schema from here: http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
```
Keypad                   Keyboard
+-+-+-+-+                +-+-+-+-+
|1|2|3|C|                |1|2|3|4|
+-+-+-+-+                +-+-+-+-+
|4|5|6|D|                |Q|W|E|R|
+-+-+-+-+       =>       +-+-+-+-+
|7|8|9|E|                |A|S|D|F|
+-+-+-+-+                +-+-+-+-+
|A|0|B|F|                |Z|X|C|V|
+-+-+-+-+                +-+-+-+-+
```
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
|0|32|
|1|3B|
|2|22| 
|3|2C| 
|4|40| 
|5|20| 
|6|24| 
|7|36| 
|8|28| 
|9|2A| 
|A|31| 
|B|1A|
|C|16| 
|D|1E|
|E|12|
|F|14|

### CHIP8 Methods
**Constructor**:
> This method is a constructor and loads the font table into memory, resets all of the registers, stack and such.

**load**:
> Reads a binary file from the ROMS directory and loads it into memory.

**cycle**:
> This is the main loop of the emulator and deals with the fetch>decode>execute cycle. This method should occur 60 times a second.

**updateScreen**:
> Updates the screen with from the screen[] array and clears the draw flag.

**returnDraw**:
> Returns the draw flag.

**dumpRegisters**:
> Prints all of the values for registers and such to help with debuging.

**clearKeys**:
> Clears the key[] array to allow next key presses to be uodated.

Resources:
http://lazyfoo.net/tutorials/SDL/index.php   
http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/  
http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
