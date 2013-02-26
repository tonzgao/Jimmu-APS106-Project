// Zipeng Cai, Anthony Gao 999826434, Jimmy Tieu //
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

//other libraries and variable declarations //

int main(void)
{ // in pseudocode //
	system(cls);
	log = fopen(\logs\log.txt, 'w')
	if (log == NULL) {
		create log file 'w';
		fprintf("=========================================================================================")
		fprintf("										 TEAM JIMMU")
		fprintf("							Zipeng Cai, Anthony Gao, Jimmy Tieu")
		fprintf("							 	num		 999826434		num/n")
		fprintf("=========================================================================================\n")
	}

	printf("Intro");
	printf("Choose mode: 1 play generated, 2 play premade, 3 watch ai, other exit")
	scanf("%d", &mode)
	switch (mode) {
		case 1: timestamp(); printf("Enter size"); scanf("%d%d", &sizex, &sizey); grid = generate_grid(sizex, sizey); break;
		case 2: timestamp(); printf("Path pls"); scanf("%string", path); grid = read_grid(path); break;// both 1 and 2 should call possible at the end //
		case 3: timestamp(); ai_play(); main();
		default: return 0;
	}

	fprintf(log, "%d x %d", sizex, sizey)
	fprint_grid() //should just do an anti read_grid()//
	fprintf("START\nx, y, score")
	int score = 0

	while (possible() == 1) {
		printf_grid()
		printf("Current score: %d", score) // should be an anti read_grid() and should add numbers to sides of grid to indicate coordinates //
		printf("New coordinates: ")
		scanf("%d %d", &x, &y)
		score += collapse(x, y)
		fprintf(log, "%d %d %d", x, y, score)
	} 

	printf("All done!/n Your final score is %d./n", score)
	fprintf(log, "END\n\nFINAL SCORE: %d (Player)", score)

	printf("press any key to play again")
	getchar()
	main()
}

// FUNCTIONS USED //
void timestamp(void)
{
	printf("----------------------------------------------------------------------------------------")
	printf("	  NEW GAME: %string", time()) //from time.h?//
	printf("----------------------------------------------------------------------------------------\n")
}

// grids are stored in nested arrays, bottom to top, left to right. //
//[1, 2, 3; 4, 5, 6] in matlab becomes [[4, 1], [5, 2], [3, 6]]. This lets the expand and collapse functions work very well //

struct generate_grid(x, y)
{
	grid = [];
	for (i = 0, i < x; i++) {
		append(grid, [])
		for (j = 0; j < y; j++) {
			int r = randint(0, 5);
			char p = "b"
			switch (r) {
				case 0: p = "b"; append(grid[i], p); break
				case 1: p = "g"; append(grid[i], p); break
				case 2: p = "r"; append(grid[i], p); break
				case 3: p = "y"; append(grid[i], p); break
				default: append(grid[i], p); break // the previously assigned char has a higher chance of being made to prevent sucky games from being made //
			}
		}
	} 
	fprintf(log, "%d x %d/n %struct", x, y, grid)
	return grid; // dont know if children functions inherit variables from parents, grid and some other variables may have to be global //
}

struct read_grid(path)
{
	fscanf("%d %d", &x, &y)
	grid = []
	for (i = 0; i < y, i++) {
		for (j = 0, j < x, j++) {
			fscanf(path, %c, &ch);
			if (i == 0) {
				append(grid, [ch]);
			} else append_to_head(grid[i], ch); // is kind of messy but makes an elegant grid system
		}
	} 
	fprintf(log, "%d x %d/n %struct", x, y, grid)
	return grid;
}

void append_to_head(grid, ch)
{}
void fprintf_grid(void)
{}
void printf_grid(void) // todo //
{}

void possible(void)  
{
	altgrid = grid
	for e in grid:
		for ch in grid[e]:
			if (altgrid[e][ch] is not caps):
				steps = 0
				expand(e, ch, 1)
				if (steps < 1) return 1;
	return 0;
}

void expand(x, y, mode) { // complicated function, should work not sure //
	char current = altgrid[x][y]
	if (current is not caps) {
		steps++
		if (mode == 1) {
			if steps > 1; break
		}
		altgrid[x][y] = caps(altgrid[x][y])
		if (altgrid[x+1][y] == current) {
			expand(x+1, y)
		}
		if (altgrid[x-1][y] == current) {
			expand(x-1, y)
		}
		if (altgrid[x][y+1] == current) {
			expand(x, y+1)
		}
		if (altgrid[x][y-1] == current) {
			expand(x, y-1)
		} 
	}
}

int collapse(x, y) {
	steps = 0
	altgrid = grid
	expand(x, y, 2)
	for e in altgrid:
		for ch in altgrid[e]:
			if ch is caps:
				remove(altgrid[e][ch])
	grid = altgrid
	for e in grid:
		if e == []:
			remove(grid, e)
	if (steps > 1) return steps*steps;
	return 0;
} 

void ai_play(void) // just a greedy algorithm //
{
		fprintf(log, "%d x %d", sizex, sizey)
		fprint_grid() //should just do an anti read_grid()//
		fprintf("START\nx, y, score")
		int score = 0

		while (possible() == 1) {
			printf_grid() // same thing as in main //
			//maybe commentary to illustrate some things at certain moments?
			highest = 2
			hx = 0
			hy = 0
			steps = 0
			for e in grid:
				for ch in grid[e]:
					expand(e, ch)
					if steps > highest:
						highest = steps
					hx = e
					hy = ch
					steps = 0
			score += collapse(hx, hy)
			printf("Current score: %d", score)
			printf("Press any key to continue")
			getchar()
			fprintf(log, "%d %d %d", x, y, score)
		} 

		printf("All done!/n The final score is %d./n", score)
		fprintf(log, "END\n\nFINAL SCORE: %d (AI)", score)

		printf("Okay, you try now. Press any key to play\n")
		getchar()
}

// bunch of functions needed to handle lists too //