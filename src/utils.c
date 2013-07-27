#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"

state_t initState(void) {
	state_t st = {.pc = 0, .halt = 0, .reg = {0}};
	
	st.mem = calloc(65536, 1);					
	
	if (st.mem == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	
	return st;
}

// Returns 32-bit instruction that is to be read
uint32_t readInstruction(state_t st) { 
	uint32_t res = 0;
	
	for (int i = 0; i < 4; i++) {
		res |= st.mem[st.pc + i] << (8 * i);
	}
	
	return res;
}

state_t executeInstruction(uint32_t inst, state_t st) {
	uint8_t opCode = getOpcode(inst);
	st.pc += 4;
	
	if (opCode == 15 || opCode == 17) { // J-Type 
		uint16_t addr = getAddr(inst);
		
		switch(opCode) {
			case 15:
				st.pc = addr;
				break;
			case 17:
				st.reg[31] = st.pc;
				st.pc = addr;
				break;
		}
	} else if (opCode == 1 || opCode == 3 || opCode == 5 || opCode == 16 || opCode == 18) { // R-Type
		uint8_t r1 = getR1(inst);
		uint8_t r2 = getR2(inst);
		uint8_t r3 = getR3(inst);
		
		switch (opCode) {
			case 1:
				st.reg[r1] = st.reg[r2] + st.reg[r3];
				break;
			case 3:
				st.reg[r1] = st.reg[r2] - st.reg[r3];
				break;
			case 5:
				st.reg[r1] = st.reg[r2] * st.reg[r3];
				break;
			case 16:
				st.pc = st.reg[r1];
				break;
			case 18:
				fprintf(stdout, "%c", st.reg[r1]);
				break;
		}
	} else {	
		if (opCode == 0) { // Halt
			printReg(st);
			st.halt = 1;			
		} else if (opCode <= 18) { // I-Type			
			uint8_t r1 = getR1(inst);
			uint8_t r2 = getR2(inst);
			int32_t imm = signExtension(getImm(inst));
			
			switch(opCode) {
				case 2:
					st.reg[r1] = st.reg[r2] + imm;
					break;
				case 4:
					st.reg[r1] = st.reg[r2] - imm;
					break;
				case 6:
					st.reg[r1] = st.reg[r2] * imm;
					break;
				case 7:
					st.reg[r1] = st.mem[st.reg[r2 + imm]];
					break;
				case 8: 
					st.mem[st.reg[r2 + imm]] = st.reg[r1];
					break;
				case 9:
					if (st.reg[r1] == st.reg[r2])
						st.pc = (st.pc - 4) + (imm * 4);
					break;
				case 10:
					if (st.reg[r1] != st.reg[r2])
						st.pc = (st.pc - 4) + (imm * 4);
					break;
				case 11:
					if (st.reg[r1] < st.reg[r2])
						st.pc = (st.pc - 4) + (imm * 4);
				case 12:
					break;
					if (st.reg[r1] > st.reg[r2])
						st.pc = (st.pc - 4) + (imm * 4);
					break;
				case 13:
					if (st.reg[r1] <= st.reg[r2])
						st.pc = (st.pc - 4) + (imm * 4);
					break;
				case 14:
					if (st.reg[r1] >= st.reg[r2])
						st.pc = (st.pc - 4) + (imm * 4);
					break;
			}	
		} else {
			fprintf(stderr, "Invalid opcode");
		}
	}
	
	return st;
}

uint8_t getOpcode(uint32_t inst) {
	uint32_t mask = 0xFC000000;
	uint32_t res = inst & mask;
	return res >> 26;
}

uint16_t getAddr(uint32_t inst) {
	uint32_t mask = 0x03FFFFFF;
	return mask & inst; 
}

uint8_t getR1(uint32_t inst) {
	uint32_t mask = 0x03E00000;
	return (inst & mask) >> 21;
}

uint8_t getR2(uint32_t inst) {
	uint32_t mask = 0x001F0000;
	return (inst & mask) >> 16;
}

uint8_t getR3(uint32_t inst) {
	uint32_t mask = 0x0000F800;
	return (inst & mask) >> 11;
}

uint16_t getImm(uint32_t inst) {
	uint32_t mask = 0x0000FFFF;
	return (inst & mask);
}

int32_t signExtension(uint16_t val) {
	if (0x00008000 & val) {
		return 0xFFFF0000 | val;
	} else {
		return val;
	}
}

void printReg(state_t st) {
	fprintf(stderr, "PC: %i\n\n", st.pc);
	
	for (int i = 0; i < 32; i++) {
		fprintf(stderr, "R[%i]: %i\n", i, st.reg[i]);
	}
}