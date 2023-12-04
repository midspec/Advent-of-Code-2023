#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_digit(char c) { return (c >= '0' && c <= '9'); }

int parse_int(char *string, char *p) {
	int result = 0;

	// Scan back to the beginning of the number
	while(p >= string && is_digit(*p)) p--;
	p++;

	// Parse and return the number
	while(*p && is_digit(*p)) { result *= 10; result += (*p - '0'); p++; }
	return result;
}

int get_gear_ratio(char *string, size_t string_length, int width, char *p) {
	// Essensially this function checks if there are exactly two integers neighboring the gear at p, and returns their product
	// The unwrapped for loops were needed to avoid counting same integer multiple times

	// Get the index of p with pointer math and check it is within bounds
	int i = p - string;
	if(i <= 0 ||i >= string_length) { printf("pointer is pointing ouside of string\n"); exit(1); }
	
	int result = 1;
	int count = 0;

	// Parse and tally neighboring numbers to the left and right of p
	if(is_digit( *(p - 1) )) { result *= parse_int(string, p - 1); count++; }
	if(is_digit( *(p + 1) )) { result *= parse_int(string, p + 1); count++; }

	// If p is not on the very top row, parse neighboring numbers in row above p
	if(i >= width) {
		if(is_digit( *(p - width) )) { result *= parse_int(string, p - width); count++; }
		else {
			if(is_digit( *(p - width - 1) )) { result *= parse_int(string, p - width - 1); count++; }
			if(is_digit( *(p - width + 1) )) { result *= parse_int(string, p - width + 1); count++; }
		}
	}

	// If p is not on the very bottom row, parse neighboring numbers in row bellow p
	if(i <= string_length - width) {
		if(is_digit( *(p + width) )) { result *= parse_int(string, p + width); count++; }
		else {
			if(is_digit( *(p + width - 1) )) { result *= parse_int(string, p + width - 1); count++; }
			if(is_digit( *(p + width + 1) )) { result *= parse_int(string, p + width + 1); count++; }
		}
	}

	// If there were not exactly two integers neighboring the gear at p, then the gear is invalid
	if(count != 2) { return 0; }
	return result;
}

int parse_schematic_gear_ratios(char *string, size_t string_length) {
	int result = 0;
	char *p = string;

	// Get width by scanning to the first newline character
	int width = 1;
	while(*p && *p != '\n') { p++; width++; }

	// Place p on the first gear ('*')
	p = string;
	while(*p && *p != '*') p++;

	// Find the gear ratio and scan to the next gear. Repeat till complete
	while(*p) {
		result += get_gear_ratio(string, string_length, width, p);
		p++;
		while(*p && *p != '*') p++;
	}

	return result;
}

int main() {
	// Open input file
	FILE *file = fopen("input.txt", "r");
	if(file == NULL) {
		puts("Could not read input.txt");
		exit(1);
	}

	// Get file length
	fseek(file, 0, SEEK_END);
	size_t file_length = ftell(file);
	rewind(file);
	if(file_length < 0) {
		puts("Error getting file length");
		exit(1);
	}

	// Allocate memory on the heap and add nul terminator
	char *input_buffer = malloc(file_length + 1);
	input_buffer[file_length] = '\0';
	fread(input_buffer, 1, file_length, file);
	fclose(file);

	// Actual Advent of Code algorithm starts here
	int sum = parse_schematic_gear_ratios(input_buffer, file_length);
	
	puts("\n|=%=%=%=%=%= Advent of Code Day 3 Part 1 =%=%=%=%=%|");
	printf("|Answer: %42d|\n", sum);

	free(input_buffer);
	return 0;
}
