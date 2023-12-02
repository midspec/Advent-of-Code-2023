#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strstrrev(const char * haystack, const char * needle) {
	// This function is identical to strstr, but it searches for the last occurrence of a substring
	char *result = NULL;
	char *next_occurrence = strstr(haystack, needle);

	while(next_occurrence != NULL) {
		haystack = next_occurrence + 1;
		result = next_occurrence;
		next_occurrence = strstr(haystack, needle);
	}

	return result;
}

int get_calibration_value(char *string, size_t string_length) {
	char *numbers[10] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
	char *found_substring;
	int first_digit = -1;
	int last_digit  = -1;
	int first_index = string_length-1;
	int last_index  = 0;
	int i;

	// Loop through string to find lowest / heighest occurrence of digit character (0, 1, 2, ect)
	for(i = 0; i < string_length; i++) {
		if(string[i] >= '0' && string[i] <= '9') { first_digit = string[i] - '0'; first_index = i; break; }
	}
	for(i = string_length-1; i >= 0; i--) {
		if(string[i] >= '0' && string[i] <= '9') { last_digit = string[i] - '0'; last_index = i; break; }
	}

	// Search for each spelled-out-number in input string, keeping track of the one with the lowest index
	for(i = 0; i < 10; i++) {
		found_substring = strstr(string, numbers[i]);
		// Need weird pointer arithmetic to get found-index from strstr because nothing is easy in C
		if(found_substring != NULL && found_substring - string < first_index) {
			first_index = found_substring - string;
			first_digit = i;
		}
	}
	// Same as above but searching from the end and keeps track of largest index
	for(i = 0; i < 10; i++) {
		found_substring = strstrrev(string, numbers[i]);
		// Need weird pointer arithmetic to get found-index from strstr
		if(found_substring != NULL && found_substring - string > last_index) {
			last_index = found_substring - string;
			last_digit = i;
		}
	}

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
