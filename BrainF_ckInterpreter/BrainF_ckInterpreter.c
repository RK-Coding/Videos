//Author: RK Coding
//Description: A simple BrainF*ck Intepreter written in C
//License: Contact Me

#include <stdio.h>

#pragma warning(disable:4996)

char* load_file(char const* path) {
	char* buffer = 0;
	long length = 0;
	FILE* f = fopen(path, "r"); //Open file in read mode

	fseek(f, 0, SEEK_END);
	length = ftell(f);
	fseek(f, 0, SEEK_SET);
	buffer = (char*)malloc((length + 1) * sizeof(char));
	if (buffer)
		fread(buffer, sizeof(char), length, f);
	
	fclose(f);
	buffer[length] = '\0';

	return buffer;
}

int main(int argc, char* argv[]) {
	//Create the pointer and array of cells
	int ptr = 0;
	int mem[1024];

	for (int i = 0; i < 1024; ++i) //Set all values to zero
		mem[i] = 0;

	//Write file given at command line int char* buffer
	char* source = load_file(argv[1]);
	int char_num = 0;

	char c = source[char_num];

	while (c != '\0') { //Parse characters
		c = source[char_num];

		//Movement
		if (c == '>') {
			if (ptr != 1024) //Check if in bounds
				++ptr;
		}
		
		else if (c == '<') {
			if (ptr != 0) //Check if in bounds
				--ptr;
		}
		
		//Modify
		else if (c == '+') {
			++mem[ptr];

			if (mem[ptr] > 255) // Loop over to 0 if greater than 255
				mem[ptr] = 0;
		}

		else if (c == '-') {
			--mem[ptr];

			if (mem[ptr] < 0) // Loop over to 255 if less than 0
				mem[ptr] = 255;
		}
		
		//Loop
		else if (c == '[') {
			if (mem[ptr] == 0) { //If cell is equal than zero
				int count_open = 0;
				++char_num;

				while (c != '\0') { //Match closed bracker
					c = source[char_num];

					if (c == ']' && count_open == 0)
						break;

					else if (c == '[')
						++count_open;

					else if (c == ']')
						--count_open;

					++char_num;
				}
			}
		}
		
		else if (c == ']') {
			if (mem[ptr] > 0) { //Cell is greater than zero
				int count_closed = 0;
				--char_num;

				while (c != '\0') { //Match open bracket
					if (char_num > 0) //Is index in bounds;
						c = source[char_num];

					if (c == '[' && count_closed == 0)
						break;

					else if (c == ']')
						++count_closed;

					else if (c == '[')
						--count_closed;

					--char_num;
				}
			}
		}

		//IO
		else if (c == '.') 
			printf("%c", mem[ptr]); 
		
		else if (c == ',')
			mem[ptr] = getchar(c);

		++char_num;
	}
}
