#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void skip_whitespace(char **p) {
	while(**p == ' ' || **p == ',' || **p == ';' || **p == ':') { *p += 1; }
}

int parse_int(char **p) {
	// If the current char is not a digit, quit;
	if(**p < '0' || **p > '9') { printf("Expected an digit, got '%c'\n", **p); return -1; }
	int result = 0;

	while(**p >= '0' && **p <= '9') {
		result *= 10;
		result += (**p) - '0';
		*p += 1;
	}
	return result;
}

bool parse_and_check_cube_amounts(char **p) {
	int cube_count = -1;

	while(**p && **p != ';') {
		cube_count = parse_int(p);
		skip_whitespace(p);
		switch(**p) {
			// Parse cube name (which can only begin with an 'r' 'g' or 'b'
			case 'r':
				if(cube_count > 12) { return false; }
				*p += 3; break;
			case 'g':
				if(cube_count > 13) { return false; }
				*p += 5; break;
			case 'b':
				if(cube_count > 14) { return false; }
				*p += 4; break;
			default: printf("Expected 'r' 'g' or 'b', but got '%c'\n", **p);
		}
		skip_whitespace(p);
	}
	return true;
}

int get_game_id_if_possible(char *string) {
	char *p = string; // This keeps track of the current positon of the parser
	p += 5;           // First 5 chars are always "Game "

	// Parse the game id
	int game_id = parse_int(&p);
	skip_whitespace(&p);

	// Check every semicolon-separated cube amounts for impossibilities. Exit while-loop if any are impossible
	bool game_is_possible = true;
	while(*p) {
		if(!parse_and_check_cube_amounts(&p)) { game_is_possible = false; break; }
	}

	// Add game id to sum only if it is possible
	return game_is_possible ? game_id : 0;
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
		sum += get_game_id_if_possible(line);
	}

	puts("\n|=%=%=%=%=%= Advent of Code Day 2 Part 1 =%=%=%=%=%|");
	printf("|Answer: %42d|\n", sum);

	free(line);
	fclose(file);
	return 0;
}
