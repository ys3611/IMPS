#ifndef UTILS_H
#define UTILS_H

typedef struct state {
	uint8_t *mem; //Must allocate on heap as it is too large for the stack
	uint16_t pc;
	int32_t reg[32]; //Easily allocated on the stack
	int halt;
} state_t;

state_t initState(void);
state_t executeInstruction(uint32_t inst, state_t st);
uint32_t readInstruction(state_t st);
uint8_t getOpcode(uint32_t inst);
uint16_t getAddr(uint32_t inst);
uint8_t getR1(uint32_t inst);
uint8_t getR2(uint32_t inst);
uint8_t getR3(uint32_t inst);
uint16_t getImm(uint32_t inst);
int32_t signExtension(uint16_t val);
void printReg(state_t st);

#endif