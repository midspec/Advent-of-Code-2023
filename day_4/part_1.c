#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void skip_whitespace(char **p) {
	while(**p && (**p < '0' || **p > '9') && **p != '|') { (*p)++; }
}

int parse_int(char **p) {
	if(**p < '0' || **p > '9') { printf("Error at: %c\n", **p); exit(1); }
	int result = 0;
	while(**p >= '0' && **p <= '9') { result *= 10; result += **p - '0'; (*p)++; }
	return result;
}

int get_card_points(char *string) {
	const int winning_numbers_length = 10;//10;
	short winning_numbers[ winning_numbers_length ];
	char *p = string;
	int i = 0;

	// Skip the game id number
	skip_whitespace(&p);
	parse_int(&p);
	skip_whitespace(&p);

	// Cach all of the winning numbers
	while(*p && *p != '|') {
		winning_numbers[i++] = parse_int(&p);
		skip_whitespace(&p);
	}
	p+=2;
	skip_whitespace(&p);

	// Search for matches and tally up points
	int points = 0;
	int number;
	while(*p) {
		number = parse_int(&p);
		skip_whitespace(&p);
		for(i = 0; i < winning_numbers_length; i++) {
			if(number == winning_numbers[i]) { points = points == 0 ? 1 : points + points; }
		}
	}
	return points;
}

int main() {
	// Open input file
	FILE *file = fopen("input.txt", "r");
	if(file == NULL) {
		puts("Could not read input.txt");
		exit(1);
	}

	// Variables for reading file line-by-line
	char *line = NULL;
	size_t buffer_size = 0;
	int line_length;

	// Actual Advent of Code algorithm starts here
	// Read file line-by-line and sum up the possible games
	int sum = 0;
	while((line_length = getline(&line, &buffer_size, file)) != -1) {
		if(line[line_length-1] == '\n') { line[--line_length] = '\0'; } // remove newline characters
		sum += get_card_points(line);
	}

	puts("\n|=%=%=%=%=%= Advent of Code Day 4 Part 1 =%=%=%=%=%|");
	printf("|Answer: %42d|\n", sum);

	free(line);
	fclose(file);
	return 0;
}
