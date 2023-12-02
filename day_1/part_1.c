#include <stdio.h>
#include <stdlib.h>

int get_calibration_value(char *string, size_t string_length) {
	int first_digit = -1;
	int last_digit  = -1;
	int i;

	// Loop through string to find lowest / heighest occurrence of digit character (0, 1, 2, ect)
	for(i = 0; i < string_length; i++) {
		if(string[i] >= '0' && string[i] <= '9') { first_digit = string[i] - '0'; break; }
	}
	for(i = string_length-1; i >= 0; i--) {
		if(string[i] >= '0' && string[i] <= '9') { last_digit = string[i] - '0'; break; }
	}

	// Erro if string did not have any digit characters in it
	if(first_digit < 0 || last_digit < 0) { printf("Error encountered at line: %s", string); exit(1); }
	return first_digit * 10 + last_digit;
}

int main() {
	// Opening input file
	FILE *file;
	char *filename = "input.txt";
	file = fopen(filename, "r");
	if(file == NULL) {
		printf("Could not read file '%s'\n", filename);
		exit(1);
	}

	// Vars for reading file line by line
	size_t buffer_length = 0;
	size_t line_length;
	char *line = NULL;

	// Actual AOC logic starts here
	int sum = 0;
	while((line_length = getline(&line, &buffer_length, file)) != -1) {
		sum += get_calibration_value(line, line_length);
	}
	puts("\n|=%=%=%=%=%= Advent of Code Day 1 Part 2 =%=%=%=%=%|");
	printf("|Answer: %42d|\n", sum);

	// Cleaning up memory and returning
	fclose(file);
	if(line) {
		free(line);
	}
	return 0;
}
