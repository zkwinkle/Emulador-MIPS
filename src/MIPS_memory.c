#include "MIPS_memory.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void init_memory(){
	RB = malloc(32*sizeof(uint32_t)); // 32 registros
	memset(RB, 0, 32*sizeof(uint32_t));
	RB[28] = 0x10008000; // $gp
	RB[29] = 0x7fffeffc; // $sp
	// $pc is just the file position for the text.hex, it's taken track of in text_manager

	data = malloc(1024*sizeof(uint32_t)); // 4 kB por la cantidad de líneas que vienen en el data.hex
	memset(data, 0, 1024*sizeof(uint32_t));
	load_data();

	heap = malloc(8192*sizeof(uint32_t)); // 32 kB
	memset(heap, 0, 8192*sizeof(uint32_t));

	stack = malloc(8192*sizeof(uint32_t)); // 32 kB
	memset(stack, 0, 8192*sizeof(uint32_t));

	MMIO = malloc(4*sizeof(uint32_t)); // Really only need the first 2 addresses
	memset(MMIO, 0, 4*sizeof(uint32_t));
}

void load_data(){
	FILE *dataStream = fopen("data.hex", "r");
	uint32_t word;
	size_t index = 0;
	while(fscanf(dataStream, "%X", &word) != EOF && index < 1024){
		data[index] = word;
	}
	fclose(dataStream);
}

void free_memory(){
	free(RB);
	free(data);
	free(heap);
	free(stack);
	free(MMIO);
}

void storeInRegister(int reg, uint32_t word){
	if(reg == 0)
		return;
	RB[reg] = word;
}

uint32_t getFromRegister(int reg){
	if(reg>=32 || reg<0){
		printf("\n-------------------------\n\n");
		printf("Intentando accesar registro no válido '%d'\n", reg);
		printf("\n-------------------------\n\n");
	}
	return RB[reg];
}

int storeInMemory(uint32_t address, uint32_t word){
	// stack goes from 0x7fffeffc to 0x7fff6ffc
	// heap goes from 0x10008000 to 0x1000ffff 
	// static data goes from 0x10010000 to 0x10010fa0
	// MMIO goes from 0xffff0000 to 0xffff0010
	
	// first check divisibility by 4
	if (address%4 != 0){
		printf("Adress given to storeInMemory '%X' is not a multiple of 4 so it can't store the given data\n", address);
		return 1;
	}
	if (address <= 0x7fffeffc && address > 0x7fff6ffc){
		uint32_t realAddress = (0x7fffeffc-address)/4;
		stack[realAddress] = word;
	}
	else if(address >= 0x10008000 && address < 0x1000ffff){
		uint32_t realAddress = (address-0x10008000)/4;
		printf("saving in heap!! index %d value %X\n", realAddress, word);
		heap[realAddress] = word;
	}
	else if(address >= 0x10010000 && address < 0x10010fa0){
		uint32_t realAddress = (address-0x10010000)/4;
		data[realAddress] = word;
	}
	else if(address >= 0xffff0000 && address < 0xffff0010){
		uint32_t realAddress = (address-0xffff0000)/4;
		MMIO[realAddress] = word;
	}
	else{
		printf("Address '%X' is outside the given ranges for the types of memory and the emulator can't deal with it (inside storeInMemory function)\n", address);
		return 1;
	}
	return 0;
}

uint32_t getFromMemory(uint32_t address){
	if (address%4 != 0){
		printf("Adress given to getFromMemory '%X' is not a multiple of 4 so it can't retrieve data from the given address\n", address);
		return -1;
	}
	if (address <= 0x7fffeffc && address > 0x7fff6ffc){
		uint32_t realAddress = (0x7fffeffc-address)/4;
		return stack[realAddress];
	}
	else if(address >= 0x10008000 && address < 0x1000ffff){
		uint32_t realAddress = (address-0x10008000)/4;
		return heap[realAddress];
	}
	else if(address >= 0x10010000 && address < 0x10010fa0){
		uint32_t realAddress = (address-0x10010000)/4;
		return data[realAddress];
	}
	else if(address >= 0xffff0000 && address < 0xffff0010){
		uint32_t realAddress = (address-0xffff0000)/4;
		return MMIO[realAddress];
	}
	else{
		printf("Address '%X' is outside the given ranges for the types of memory and the emulator can't deal with it (inside getFromMemory function)\n", address);
		return -1;
	}
}

uint32_t getFromHeap(size_t index){
	return heap[index];
}
