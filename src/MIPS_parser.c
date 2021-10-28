#include "MIPS_parser.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "bit_manipulation.h"
#include "MIPS_instructions.h"

static char* name; // for printing function names

// parsing instructions
int instructionParse(uint32_t instruction, size_t index){
	//usleep(20000); // pause for 20ms for better visualization
	char format = 'I'; // I, R or J

	int exe = 1;

	uint32_t opcode = getBitRange(instruction, 32, 26);
	uint32_t rs = getBitRange(instruction, 26, 21);
	uint32_t rt = getBitRange(instruction, 21, 16);
	uint16_t immediate = getBitRange(instruction, 16, 0);
	uint32_t address = getBitRange(instruction, 26, 0);
	
	//instrucciones comentadas no se utilizan para códigon de pong
	switch(opcode){
		case 0x0: // tipo R
			format = 'R';
			if(RParse(instruction, exe)){
				printf("funct '%X' de instrucción tipo R no reconocido en línea %d",getBitRange(instruction, 6, 0), index);
				return 1;
			}
			break;

		case 0x8: // addi
			name = "addi";

			if(exe)
				add_imm(rt, rs, immediate);
			break;

		case 0x9: // addiu
			name = "addiu";

			if(exe)
				add_imm_unsigned(rt, rs, immediate);
			break;

		//case 0xC: // andi
		//	name = "andi";
		//	break;

		case 0x4: // beq
			name = "beq";

			if(exe)
				branch_on_equal(rs, rt, immediate);
			break;

		case 0x5: // bne
			name = "bne";

			if (exe)
				branch_on_not_equal(rs, rt, immediate);
			break;

		case 0x06: // blez
			name = "blez";

			if(exe)
				branch_on_less_equal_zero(rs, immediate);
			break;

		case 0x2: // j
			name = "j";
			format = 'J';

			if(exe)
				jump(address);
			break;

		case 0x3: // jal
			name = "jal";
			format = 'J';

			if(exe)
				jump_and_link(address);
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

			if(exe)
				load_upper_imm(rt, immediate);
			break;

		case 0x23: // lw
			name = "lw";

			if(exe)
				load_word(rt, immediate, rs);
			break;

		case 0x2B: // sw
			name = "sw";

			if(exe)
				if(store_word(rt, immediate, rs))
				return 1;
			break;

		case 0xD: // ori
			name = "ori";

			if(exe)
				or_imm(rt, rs, immediate);
			break;

		case 0xA: // slti
			name = "slti";

			if(exe)
				set_less_than_imm(rt, rs, immediate);
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

int RParse(uint32_t instruction, int exe){ 

	uint32_t rs = getBitRange(instruction, 26, 21);
	uint32_t rt = getBitRange(instruction, 21, 16);
	uint32_t rd = getBitRange(instruction, 16, 11);
	uint32_t shamt = getBitRange(instruction, 10, 6);
	uint32_t funct = getBitRange(instruction, 6, 0);

	// Los functs comentados no se utilizan en Pong
	switch(funct){
		case 0x20: // add
			name = "add";

			if(exe)
				add(rd,  rs,  rt);
			break;

		case 0x21: // addu
			name = "addu";

			if(exe)
				add_unsigned(rd,  rs,  rt);
			break;

		//case 0x24: // and
		//	name = "and";
		//	break;

		case 0x08: // jr
			name = "jr";

			if(exe)
				jump_register(rs);
			break;

		//case 0x27: // nor
		//	name = "nor";
		//	break;

		//case 0x25: // or
		//	name = "or";
		//	break;

		case 0x2A: // slt
			name = "slt";

			if(exe)
				set_less_than(rd,  rs,  rt);
			break;

		//case 0x2B: // sltu
		//	name = "sltu";
		//	break;

		case 0x00: // sll
			name = "sll";

			if(exe)
				shift_left_logical(rd,  rt,  shamt);
			break;

		//case 0x02: // srl
		//	name = "srl";
		//	break;

		case 0x22: // sub
			name = "sub";

			if(exe)
				sub(rd,  rs,  rt);
			break;

		//case 0x23: // subu
		//	name = "subu";
		//	break;
		
		case 0x26: // xor
			name = "xor";

			if(exe)
				xor(rd,  rs,  rt);
			break;

		case 0xC: // syscall
			name = "syscall";

			if(exe && mips_syscall())
				return 1;
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
			else if(opcode == 0xf)
				printf("%s $%s, %X", name, rt, (uint16_t) imm);
			else if(opcode == 0x23 || opcode == 0x24 || opcode == 0x25 || opcode == 0x28 || opcode == 0x29 || opcode == 0x2b)
				printf("%s $%s, %d($%s)", name, rt, imm , rs);
			else if(opcode == 0x4 || opcode == 0x5)
				printf("%s $%s, $%s, %d", name, rs, rt, imm);
			else
				printf("%s $%s, $%s, %d", name, rt, rs, imm);
			break;
		case 'J':
			printf("%s 0x%.8X", name, getBitRange(instruction, 26, 0)<<2); // shift left by 2 porque MIPS le corta los últimos 2 0s
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
