#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct { int red_cubes; int green_cubes; int blue_cubes; } MinCubeSet;

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

void parse_cube_amounts(char **p, MinCubeSet *min_cubes) {
	int cube_count = -1;

	while(**p && **p != ';') {
		cube_count = parse_int(p);
		skip_whitespace(p);
		switch(**p) {
			// Parse cube name (which can only begin with an 'r' 'g' or 'b'
			case 'r':
				if(cube_count > min_cubes->red_cubes) min_cubes->red_cubes = cube_count;
				*p += 3; break;
			case 'g':
				if(cube_count > min_cubes->green_cubes) min_cubes->green_cubes = cube_count;
				*p += 5; break;
			case 'b':
				if(cube_count > min_cubes->blue_cubes) min_cubes->blue_cubes = cube_count;
				*p += 4; break;
			default: printf("Expected 'r' 'g' or 'b', but got '%c'\n", **p);
		}
		skip_whitespace(p);
	}
}

int parse_game_power(char *string) {
	char *p = string; // This keeps track of the current positon of the parser
	p += 5;           // First 5 chars are always "Game "

	// Parse the game id
	int game_id = parse_int(&p);
	skip_whitespace(&p);

	// Loop throuogh every cube draw, keeping track of the minimum required cube amounts
	MinCubeSet min_cubes = { 0, 0, 0 };
	while(*p) {
		parse_cube_amounts(&p, &min_cubes);
	}

	// Return the power of the min cube set (product of the min cube amounts)
	return min_cubes.red_cubes * min_cubes.green_cubes * min_cubes.blue_cubes;
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
		sum += parse_game_power(line);
	}

	puts("\n|=%=%=%=%=%= Advent of Code Day 2 Part 2 =%=%=%=%=%|");
	printf("|Answer: %42d|\n", sum);

	free(line);
	fclose(file);
	return 0;
}
