#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

// This file manages the $pc and reads the hex text file 

#include <stdio.h>
#include <stdint.h>

extern FILE *textStream;
extern uint32_t pc;

void openText();
void closeText();

int readNextInstruction();
void jumpToInstruction(uint32_t address);

#endif
