#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"

// Will open the file, and using a loop will go through all of the instructions
int main(int argc, char **argv) {
	FILE *fp;
	long size;
	
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	
	state_t st = initState();
	
	fseek(fp, 0, SEEK_END);
	size = ftell(fp) / 4;
	fseek(fp, 0, SEEK_SET);

	fread(st.mem, 4, size, fp);
	
	while (st.halt == 0) {
		st = executeInstruction(readInstruction(st), st);
	}
	
	fclose(fp);
	free(st.mem);
	
	return EXIT_SUCCESS;
}