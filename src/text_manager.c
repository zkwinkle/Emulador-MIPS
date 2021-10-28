#include "text_manager.h"
#include "MIPS_parser.h"

FILE *textStream = NULL;
uint32_t pc = 0;

void openText(){
	textStream = fopen("text.hex", "r");
	pc = 0;
}

int readNextInstruction(){
	uint32_t instruction = 0;
	if(fscanf(textStream, "%X", &instruction) != EOF && !instructionParse(instruction, pc)){
		pc+=1;
		return 0;
	}
	else
		return 1;
}

void closeText(){
	fclose(textStream);
}

void jumpToInstruction(uint32_t address){
	// $pc starts at 0x0040000 on MIPS and instructions adresses skip 4 by 4
	pc = (address - 0x00400000)/4;
	fseek(textStream, pc*9, SEEK_SET); // *9 porque cada línea son 9 bytes (8 caracteres hex + \n)
}

void relativeJump(int16_t from_pc4){
	pc = pc + from_pc4; // pc is already the current instruction + 1
	fseek(textStream, pc*9, SEEK_SET);
}
