#include <parser.h>

// parsing instructions
int instructionParse(uint32_t instruction, size_t index){
	uint32_t opcode = getBitRange(instruction, 32, 26);
	char  // I, R or J
	switch(opcode){
		case 0x0: // tipo R
			format = 'R';
			uint32_t funct = getBitRange(instruction, 6, 0);
			switch(funct)
			break;

		case 0x8: // addi
			break;
			
		case 0x9: // addiu
			break;
			
		case 0xC: // andi
			break;

		case 0x4: // beq
			break;

		case 0x5: // bne
			break;

		case 0x2: // j 
			format = 'J';
			break;

		case 0x3: // jal
			format = 'J';
			break;

		case 0x24: // lbu
			break;

		case 0x25: // lhu
			break;

		case 0x30: // ll
			break;

		case 0xf: // lui
			break;

		case 0x23: // lw
			break;

		case 0xD: // ori
			break;

		case 0xA: // slti
			break;

		case 0xB: // sltiu
			break;

		case 0x28: // sb
			break;

		case 0x38: // sc
			break;

		case 0x29: // sh 
			break;

		case 0x2B: // sw
			break;

		default:
			printf("opcode '%X' no reconozido en línea %d",opcode, index);
			return 1;
	}
	return 0;
}
