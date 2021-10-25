#include <MIPS_parser.h>
#include <stdio.h>
#include <string.h>

#include <bit_manipulation.h>

static char* name; // for printing function names

// parsing instructions
int instructionParse(uint32_t instruction, size_t index){
	uint32_t opcode = getBitRange(instruction, 32, 26);
	char format = 'I'; // I, R or J
	
	//instrucciones comentadas no se utilizan para códigon de pong
	switch(opcode){
		case 0x0: // tipo R
			format = 'R';
			if(RParse(instruction)){
				printf("funct '%X' de instrucción tipo R no reconocido en línea %d",getBitRange(instruction, 6, 0), index);
				return 1;
			}
			break;

		case 0x8: // addi
			name = "addi";
			break;

		case 0x9: // addiu
			name = "addiu";
			break;

		//case 0xC: // andi
		//	name = "andi";
		//	break;

		case 0x4: // beq
			name = "beq";
			break;

		case 0x5: // bne
			name = "bne";
			break;

		case 0x2: // j
			name = "j";
			format = 'J';
			break;

		case 0x3: // jal
			name = "jal";
			format = 'J';
			break;

		//case 0x24: // lbu
		//	name = "lbu";
		//	break;

		//case 0x25: // lhu
		//	name = "lhu";
		//	break;

		////case 0x30: // ll
		////	name = "ll";
		////	break;

		case 0xf: // lui
			name = "lui";
			break;

		case 0x23: // lw
			name = "lw";
			break;

		case 0xD: // ori
			name = "ori";
			break;

		case 0xA: // slti
			name = "slti";
			break;

		//case 0xB: // sltiu
		//	name = "sltiu";
		//	break;

		//case 0x28: // sb
		//	name = "sb";
		//	break;

		////case 0x38: // sc
		////	name = "sc";
		////	break;

		//case 0x29: // sh
		//	name = "sh";
		//	break;

		case 0x2B: // sw
			name = "sw";
			break;

		case 0x06: // blez
			name = "blez";
			break;

		//case 0x07: // bgtz
		//	name = "bgtz";
		//	break;

		default:
			printf("opcode '%X' no reconocido en línea %d\nInstrucción: %X\n",opcode, index, instruction);
			return 1;
	}
	printInstruction(instruction, format, name, index);
	return 0;
}

int RParse(uint32_t instruction){ 
	uint32_t funct = getBitRange(instruction, 6, 0);

	// Los functs comentados no se utilizan en Pong
	switch(funct){
		case 0x20: // add
			name = "add";
			break;

		case 0x21: // addu
			name = "addu";
			break;

		//case 0x24: // and
		//	name = "and";
		//	break;

		case 0x08: // jr
			name = "jr";
			break;

		//case 0x27: // nor
		//	name = "nor";
		//	break;

		//case 0x25: // or
		//	name = "or";
		//	break;

		case 0x2A: // slt
			name = "slt";
			break;

		//case 0x2B: // sltu
		//	name = "sltu";
		//	break;

		case 0x00: // sll
			name = "sll";
			break;

		//case 0x02: // srl
		//	name = "srl";
		//	break;

		case 0x22: // sub
			name = "sub";
			break;

		//case 0x23: // subu
		//	name = "subu";
		//	break;
		
		case 0x26: // xor
			name = "xor";
			break;

		case 0xC: // syscall
			name = "syscall";
			break;

		default:
			printf("funct '%X' de instrucción R no reconocido\n", funct);
			return 1;
	}
	return 0;
}

int printInstruction(uint32_t instruction, char format, char* name, size_t index){
	char rs[5], rt[5], rd[5];
	num2reg(getBitRange(instruction,26,21), rs);
	num2reg(getBitRange(instruction,21,16), rt);
	num2reg(getBitRange(instruction,16,11), rd);
	printf("%03d  ",index);

	uint32_t opcode = getBitRange(instruction, 32, 26);
	printf("%c: ", format);

	switch(format){
		case 'R':
			uint32_t funct = getBitRange(instruction, 6, 0);
			if(funct == 0x08)
				printf("%s $%s", name, rs);
			else if(funct == 0x00 || funct == 0x02)
				printf("%s $%s, $%s, %d", name, rd, rt, getBitRange(instruction, 11, 6));
			else if(funct == 0xC)
				printf("syscall");
			else
				printf("%s $%s, $%s, $%s", name, rd, rs, rt);
			break;
		case 'I':
			int16_t imm = getBitRange(instruction, 16, 0);
			if(opcode == 0x06 || opcode == 0x07)
				printf("%s $%s, %d", name, rs, imm);
			else if(opcode == 0x23 || opcode == 0x24 || opcode == 0x25 || opcode == 0x28 || opcode == 0x29 || opcode == 0x2b)
				printf("%s $%s, %d($%s)", name, rt, imm , rs);
			else
				printf("%s, $%s, $%s, $%d", name, rs, rt, imm);
			break;
		case 'J':
			printf("%s %X", name, getBitRange(instruction, 26, 0));
			break;
	}
	printf("\n");
}

void num2reg(int reg, char* buffer){
	// Código muy inseguro, asegurarse que el buffer es de tamaño 5
	if(reg == 0)
		memcpy(buffer, "zero", 5);
	else if(reg == 1)
		memcpy(buffer, "at", 5);
	else if(reg <= 3)
		snprintf(buffer, 5, "v%d", reg-2);
	else if(reg <=7)
		snprintf(buffer, 5, "a%d", reg-4);
	else if(reg <=15)
		snprintf(buffer, 5, "t%d", reg-8);
	else if(reg <=23)
		snprintf(buffer, 5, "s%d", reg-16);
	else if(reg <=25)
		snprintf(buffer, 5, "t%d", reg-24);
	else if(reg <=27)
		snprintf(buffer, 5, "k%d", reg-26);
	else if(reg == 28)
		memcpy(buffer, "gp", 5);
	else if(reg == 29)
		memcpy(buffer, "sp", 5);
	else if(reg == 30)
		memcpy(buffer, "fp", 5);
	else if(reg == 31)
		memcpy(buffer, "ra", 5);
	else
		memcpy(buffer, "NULL", 5);
}
