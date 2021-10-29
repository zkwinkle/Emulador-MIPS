#include "MIPS_instructions.h"

#include <unistd.h>

#include "MIPS_memory.h"
#include "text_manager.h"
#include "bit_manipulation.h"

// R type instructions
void add(uint32_t rd, uint32_t rs, uint32_t rt){
	storeInRegister(rd, (int32_t) getFromRegister(rs) + (int32_t) getFromRegister(rt));
}

void add_unsigned(uint32_t rd, uint32_t rs, uint32_t rt){
	//printf("rd '%X' rs '%X' rt '%X'\n", getFromRegister(rd), getFromRegister(rs), getFromRegister(rt));
	storeInRegister(rd, getFromRegister(rs) + getFromRegister(rt));
}

void jump_register(uint32_t rs){
	jumpToInstruction(getFromRegister(rs));
}

void set_less_than(uint32_t rd, uint32_t rs, uint32_t rt){
	storeInRegister(rd, (uint32_t) ((int32_t) getFromRegister(rs) < (int32_t) getFromRegister(rt)) );
}

void shift_left_logical(uint32_t rd, uint32_t rt, uint32_t shamt){
	//printf("sll shifts %d for result: %X\n", shamt, getFromRegister(rt) << shamt);
	storeInRegister(rd, getFromRegister(rt) << shamt);
}

void sub(uint32_t rd, uint32_t rs, uint32_t rt){
	storeInRegister(rd, (int32_t) getFromRegister(rs) - (int32_t) getFromRegister(rt));
}

void xor(uint32_t rd, uint32_t rs, uint32_t rt){
	//printf("xor between '%X' and '%X'\n", getFromRegister(rs), getFromRegister(rt));
	storeInRegister(rd, getFromRegister(rs) ^ getFromRegister(rt));
}


// syscall
int mips_syscall(){
	int service = getFromRegister(2); // load service number from $v0
	if(service == 31){
		// MIDI out, probs do this last
		;
	}
	else if(service == 32){
		// sleep for $a0 milliseconds
		//printf("sleeping for %X\n" , getFromRegister(4));
		size_t millis = getFromRegister(4);
		usleep(millis*1000);
	}
	else{
		//printf("syscall %d unrecognized\n", service);
		return 1;
	}
	return 0;

}


// I type instructions
void add_imm(uint32_t rt, uint32_t rs, uint16_t imm){
	storeInRegister(rt, (int32_t) getFromRegister(rs) + ((int32_t) (int16_t) imm));
}

void add_imm_unsigned(uint32_t rt, uint32_t rs, uint16_t imm){
	storeInRegister(rt, getFromRegister(rs) + ((uint32_t)  (int16_t) imm) );
}


void branch_on_equal(uint32_t rs, uint32_t rt, uint16_t imm){
	if(getFromRegister(rs) == getFromRegister(rt)){
		relativeJump((int16_t) imm);
	}
}

void branch_on_not_equal(uint32_t rs, uint32_t rt, uint16_t imm){
	if(getFromRegister(rs) != getFromRegister(rt))
		relativeJump((int16_t) imm);
}

void branch_on_less_equal_zero(uint32_t rs, uint16_t imm){
	if(getFromRegister(rs) <= 0)
		relativeJump((int16_t) imm);
}


void load_upper_imm(uint32_t rt, uint16_t imm){
	uint32_t upper_imm = (uint32_t) imm << 16;
	storeInRegister(rt, upper_imm);
}


void load_word(uint32_t rt, uint16_t imm, uint32_t rs){
	//printf("word '%X' \n", getFromMemory(getFromRegister(rs) + (int16_t) imm));
	storeInRegister(rt, getFromMemory(getFromRegister(rs) + (int16_t) imm));
}

int store_word(uint32_t rt, uint16_t imm, uint32_t rs){
	//printf("sw in address '%X' the word '%X'\n", getFromRegister(rs) + (int16_t) imm, getFromRegister(rt));
	return storeInMemory(getFromRegister(rs) + (int16_t) imm, getFromRegister(rt));
}


void or_imm(uint32_t rt, uint32_t rs, uint16_t imm){
	storeInRegister(rt, getFromRegister(rs) | imm);
}

void set_less_than_imm(uint32_t rt, uint32_t rs, uint16_t imm){
	storeInRegister(rt, (uint32_t) ((int32_t) getFromRegister(rs) < (int16_t) imm) );
}


// J type instructions
void jump(uint32_t address){
	uint32_t newAddress = (getBitRange(pc, 32, 28) << 28)+(address<<2);
	jumpToInstruction(newAddress);
}

void jump_and_link(uint32_t address){
	storeInRegister(31, pc*4 + 0x00400000);
	jump(address);
}

