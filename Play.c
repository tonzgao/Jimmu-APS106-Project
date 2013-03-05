// Zipeng Cai, Anthony Gao 999826434, Jimmy Tieu //
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

//other libraries and variable declarations //
char player[25] = {0};
time_t btime;

int main(void)
{
    FILE * log = fopen("log.txt", "a+");
    char empty = "a"
    fscanf(log, "%c", &empty);
	if (empty != "/=") {
		fprintf(log, "===============================================================================\n");
		fprintf(log, "									TEAM JIMMU\n");
		fprintf(log, "				Zipeng Cai, Anthony Gao, Richard Shangguan, Jimmy Tieu\n");
		fprintf(log, "studentnums\n");
		fprintf(log, "===============================================================================\n\n");
	}
	
    printf("What is your name?");
    scanf("%25[0-9a-zA-Z ]s", &player);
    play();
    return 0;
}

int play(void)
{
	system(cls);
    printf("Intro");
	printf("Choose mode: 1 play generated, 2 play premade, 3 watch ai play generated, 4 watch ai play premade, other exit")
	scanf("%d", &mode)
	switch (mode) {
		case 1: timestamp(); printf("Enter size"); scanf("%d%d", &sizex, &sizey); grid = generate_grid(sizex, sizey); break;
		case 2: timestamp(); printf("Path pls"); scanf("%s", path); grid = read_grid(path); break;// both 1 and 2 should call possible at the end //
		case 3: timestamp(); printf("Enter size"); scanf("%d%d", &sizex, &sizey); grid = generate_grid(sizex, sizey); ai_play(); play(); break;
		case 4: timestamp(); printf("Path pls"); scanf("%s", path); grid = read_grid(path); ai_play(); play(); break;
		default: exit(0);
	}

	fprintf(log, "%d x %d", sizex, sizey)
	fprint_grid()
	fprintf("START\nx, y, score")
	int score = 0

	while (possible() == 1) {
		printf_grid()
		printf("Current score: %d", score)
		printf("New coordinates: ")
		scanf("%d %d", &x, &y)
		score += collapse(x, y)
		fprintf(log, "%d %d %d", x, y, score)
	}

	printf("All done!/n Your final score is %d./n", score)
	fprintf(log, "END\n\nFINAL SCORE: %d (%s)", score, player)

	printf("press any key to play again")
	getchar()
	play()
}

// FUNCTIONS USED //
void timestamp(void)
{
    time (&btime);
	fprintf(log, "-------------------------------------------------------------------------------\n");
	fprintf(log, "	  NEW GAME: %s", ctime(&btime));
	fprintf(log, "-------------------------------------------------------------------------------\n\n");
}

// grids are stored in nested arrays, bottom to top, left to right. //
//[1, 2, 3; 4, 5, 6] in matlab becomes [[4, 1], [5, 2], [3, 6]]. This lets the expand and collapse functions work very well //

dunno generate_grid(x, y)
{
	grid = [];
	for (i = 0, i < x; i++) {
		append(grid, [])
		for (j = 0; j < y; j++) {
			//seedwithtime();
			int r = rand(0, 5);
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
	fprintf(log, "%d x %d/n %s", x, y, read(grid)
	return grid; // dont know if children functions inherit variables from parents, grid and some other variables may have to be global //
}

dunno read_grid(path)
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
	fprintf(log, "%d x %d/n %s", x, y, print(grid)
	return grid;
}

void printf_grid(void) // convert array to a string. these two should be reversals of read_grid() //
{}

void possible(void)
{
	altgrid = grid
	for e in grid:
		for ch in grid[e]:
			if (altgrid[e][ch] is not caps):
				steps = 0
				expand(e, ch, 1)
				if (steps < 1) {return 1};
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
	for e in grid: // may need to fill empty spaces with zeros. //
		if e == []:
			remove(grid, e)
	if (steps > 1) return steps*steps;
	return 0;
}

void ai_play(void) // just a standard greedy algorithm //
{
		fprintf(log, "%d x %d", sizex, sizey)
		fprint(file, "%s", printf_grid(grid))
		fprintf("START\nx, y, score")
		int score = 0
		while (possible() == 1) {
			printf_grid()
			printf("Current score: %d", score)
			printf("Press any key to continue")
			getchar()
			//maybe commentary to illustrate some things at certain moments?
			highest = 2
			hx = 0
			hy = 0
			steps = 0
			for e in grid {// move this section in braces to a mode in collapse which removes the highest rank. makes altgrid not need to be global.
				for ch in grid[e]:
					if (altgrid[e][ch] is not caps):
						expand(e, ch, 0)
						if steps > highest:
							highest = steps
						hx = e
						hy = ch
						steps = 0
			}
			score += collapse(hx, hy)
			fprintf(log, "%d %d %d", x, y, score)
		}

		printf("All done!/n The final score is %d./n", score)
		fprintf(log, "END\n\nFINAL SCORE: %d (AI)", score)

		printf("Okay, you try now. Press any key to play\n")
		getchar()
}

void append_to_head(grid, ch) // bunch of other functions needed to handle lists too //
{}

int is_caps(ch)
{}


//  COPYRIGHT LICENCE
//  Copyright [2013] [Gary Cai, Anthony Gao, and Jimmy Tieu]
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
