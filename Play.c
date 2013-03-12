// Zipeng Cai, Anthony Gao 999826434, Richard Shangguan, Jimmy Tieu //
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

//does not compile currently. Use test.c to test out functions //
char player[25] = {0};
time_t btime;
int sizex = 0, sizey = 0, warn;

int main(void)
// Opens the log file and asks the Player's name //
{
    FILE * log;
    FILE * testfile = fopen("Log/log.txt", "r");
	if (testfile == NULL) {
	    log = fopen("Log/log.txt", "a");
		fprintf(log, "===============================================================================\n");
		fprintf(log, "									TEAM JIMMU\n");
		fprintf(log, "				Zipeng Cai, Anthony Gao, Richard Shangguan, Jimmy Tieu\n");
		fprintf(log, "studentnums\n");
		fprintf(log, "===============================================================================\n\n");
	}
	fclose(testfile);

    printf("Please input your name: ");
    scanf("%25s", &player);
    play();
    return 0;
}

int play(void)
// Player chooses mode; Preparations for each mode begin; Player can play and machine logs. Unfinished //
{
	system(cls);
    printf("Intro");
	printf("Choose mode: 1 play generated, 2 play premade, 3 watch ai play generated, 4 watch ai play premade, other exit")
	scanf("%d", &mode)
	switch (mode) {
		case 1: { // something else we could do is have seperate functions for play and ai and then if/else, rather than a switch. //
					timestamp();
				    for (warn = 0; sizex > 39 || sizey > 39 || sizex < 2 || sizey < 2; warn++) {
				        if (warn > 0) {
				            printf("Sorry, both x and y have to be between 2 and 39\n\n");
				        }
				        printf("Enter size of x: ");
				        scanf("%d", &sizex);
				        printf("Enter size of y: ");
				        scanf("%d", &sizey);
				    }
					char grid[sizex][sizey];
					generate_grid(grid);
					break;
				}
		case 2: {
					warn = -1;
                	while (warn < 0) {
	                    printf("Please input the grid path: ");
	                    scanf("%25s", &path);
                    	FILE * input = fopen(path, "r");
                    	fscanf(input, "%d %d", &sizey, &sizex);
                    	char grid[sizex][sizey];
                    	fclose(input);
                    	warn = read_grid(grid, path);
                	}
                	break;
            	}
		case 3: timestamp(); printf("Enter size"); scanf("%d%d", &sizex, &sizey); grid = generate_grid(sizex, sizey); ai_play(); play(); break;
		case 4: timestamp(); printf("Path pls"); scanf("%s", &path); grid = read_grid(path); ai_play(); play(); break;
		default: exit(0);
	}

	print_grid(grid); // problem: does not work because the scope of grid is limited to the switch statement in case 1, and the while loop in case 2. //
	fprintf("\nSTART\nx, y, score");
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

void human(sizex, sizey)
{} //see line 41 //

void timestamp(void)
// Stamps the time for each game in the logs //
{
	FILE * log;
	log = fopen("Log/log.txt", "a");
	time (&btime);
	fprintf(log, "-------------------------------------------------------------------------------\n");
	fprintf(log, "	  %s, NEW GAME: %s", player, ctime(&btime));
	fprintf(log, "-------------------------------------------------------------------------------\n\n");
	fclose(log);
}

void generate_grid(char grid[sizex][sizey])
// Generates a grid which represents like this: [1, 2, 3; 4, 5, 6] has three columns and two rows; becomes [[4, 1], [5, 2], [3, 6]]. //
{
    int i, j, random, seed;
    char p = 'b';

    FILE * log;
	log = fopen("Log/log.txt", "a");
	fprintf(log, "%d x %d\n", sizex, sizey);

    seed = time(NULL);
    srand(seed);
    for (j = sizey - 1; j >= 0; j--) {
        for (i = 0; i < sizex; i++) {
            random = rand() % (5 - (sizex*sizey)/1000);
            switch(random) {
                case 0: p = 'b'; break;
                case 1: p = 'r'; break;
                case 2: p = 'y'; break;
                case 3: p = 'g'; break;
                default: break;
            }
            grid[i][j] = p;
            fprintf(log, "%c ", p);
        }
        fprintf(log, "\n");
    }
    fclose(log);
}

int read_grid(char grid[sizex][sizey], char path[25])
{
    int i, j;
    char color = 'b';
    FILE * input = fopen(path, "r");
	FILE * log = fopen("Log/log.txt", "a");
	fprintf(log, "%d x %d\n", sizex, sizey);

    fscanf(input, "%d%d", &j, &i);
    if (i != sizex || j != sizey) {
        fclose(log);
        fclose(input);
        return -2;
    }

    for (j = sizey - 1; j >= 0; j--) {
        for (i = 0; i < sizex; i++) {
            fscanf(input, "%c", &color);
            if (color != 'b' && color != 'g' && color != 'r' && color != 'y' && color != '\n' && color != ' ') {
                fclose(log);
                fclose(input);
                return -1;
            }
            if (color == '\n' || color == ' ') {
                fprintf(log, "%c", color);
                i--;
            } else {
                grid[i][j] = color;
                fprintf(log, "%c ", color);
            }

        }
    }
    fclose(log);
    fclose(input);
    return 0;
}

void print_grid(char grid[sizex][sizey])
{
    int i, j;
    char color;
    printf(" Y \n");
    for (j = sizey - 1; j > 0; j--) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
        printf("%2d ", j+1);
        for (i = 0; i < sizex; i++) {
            color = grid[i][j];
            switch (color) {
                case 'b': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE); printf("  "); break;
                case 'r': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED); printf("  "); break;
                case 'y': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_RED); printf("  "); break;
                case 'g': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN); printf("  "); break;
                default: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN); printf("  "); break;
            }
        }
        printf("\n");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
    printf(" ");
    for (i = 0; i < sizex; i += 2) {
        printf("%4d", i+1);
    }
    if (i % 2 == 0) {
        printf(" ");
    }
    printf("  X");
}

void possible(void)
// Checks if moves are possible. Returns 0 if not, and 1 if yes. Unfinished//
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

void expand(x, y, mode) {
// Complicated function, should work not sure. Basically puts to upper each coordinate that should be removed. unfinished  //
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
// Removes all capital letters. Shuffles down coordinates that need to fall and removes empty columns. unfinished //
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

void ai_play(void)
// just a standard greedy algorithm. unfinished //
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





//  COPYRIGHT LICENCE
//   The MIT License (MIT)
//
//Copyright (c) <2013> <Zipeng Cai, Anthony Gao, Richard Shangguan, Jimmy Tieu>
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//
//IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
