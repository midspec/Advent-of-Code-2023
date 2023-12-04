#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_digit(const char c) { return (c >= '0' && c <= '9'); }

bool is_symbol_at(const int i, const char *string, const size_t string_length) {
	if(i < 0 || i >= string_length) { return false; }
	char c = string[i];
	return (!is_digit(c) && c != '.' && c != '\n');
}

void skip_non_digits(char **p, int *i) {
	while(**p && !is_digit(**p)) { (*p)++; (*i)++; }
}

int parse_schematic_part_numbers(char *string, const size_t string_length) {
	int result = 0;
	char *p = string;
	int width;
	int i;
	int number;
	bool touching_symbol;

	// Get width by scanning until hitting a newline
	for(width = 1;*p && *p != '\n'; p++) width++;

	// place p at first digit character
	p = string;
	skip_non_digits(&p, &i);

	while(*p) {
		// Check 3 cells infront of number
		number = 0;
		touching_symbol = false ||
		is_symbol_at(i-1-width, string, string_length) ||
		is_symbol_at(i-1,       string, string_length) ||
		is_symbol_at(i-1+width, string, string_length);
		
		// Parse number while also checking character above and bellow
		while(is_digit(*p)) {
			number *= 10;
			number += (*p - '0');
			touching_symbol = touching_symbol ||
			is_symbol_at(i+width, string, string_length) ||
			is_symbol_at(i-width, string, string_length);
			p++;
			i++;
		}
		// Check 3 cells at the end of the number
		touching_symbol = touching_symbol ||
		is_symbol_at(i-width, string, string_length) ||
		is_symbol_at(i,       string, string_length) ||
		is_symbol_at(i+width, string, string_length);

		// If number is touching a symbol, add it to the sum
		if(touching_symbol) { result += number; }

		skip_non_digits(&p, &i);
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
	int sum = parse_schematic_part_numbers(input_buffer, file_length);
	
	puts("\n|=%=%=%=%=%= Advent of Code Day 3 Part 1 =%=%=%=%=%|");
	printf("|Answer: %42d|\n", sum);

	free(input_buffer);
	return 0;
}
