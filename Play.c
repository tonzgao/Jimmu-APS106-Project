// Zipeng Cai, Anthony Gao 999826434, Richard Shangguan, Jimmy Tieu //
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

// File does not compile currently. Use test.c to test out functions //
char player[25] = {0};
time_t btime;
int sizex = -1, sizey = -1;

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
    scanf("%[ -~]25s", &player);
    start();
    return 0;
}

void start(void)
// Player chooses betwen playing or watching ai. After each game, the player return here //
{
    int mode = 2, warn;
    printf("\n1 Computer Generated Grid\n2 Custom Grid\n0 EXIT\n\nPlease state what you want to play: ");
    scanf("%d", &mode);
    if (mode != 1 && mode != 2) {
        exit(0);
    }

    FILE * log = fopen("Log/log.txt", "a");
    char path[25] = {0};
    timestamp();

    if (mode == 2) {
        printf("\nPlease type the path to your grid: ");
        scanf("%25s", &path);
        FILE * input = fopen(path, "r");
        fscanf(input, "%d%d", &sizey, &sizex);
        fclose(input);
    } else {
        printf("\nPlease input the size of your grid.\n");
        for (warn = 0; sizex > 36 || sizey > 45 || sizex < 7 || sizey < 7; warn++) {
            if (warn > 0) {
                printf("\nSorry, please be reasonable\n");
            }
            if (warn > 7) {
                printf("\nI feel like there is a problem here.\n");
                exit(0);
            }
            printf("\nEnter size of x: ");
            scanf("%d", &sizex);
            if (sizex == 0) {
                int random, seed;
                seed = time(NULL);
                srand(seed);
                sizex = (rand() % 20) + 8;
            }
            printf("Enter size of y: ");
            scanf("%d", &sizey);
            if (sizey == 0) {
                int random, seed;
                seed = time(NULL);
                srand(seed);
                sizey = (rand() % 25) + 8;
            }
        }
    }
    char grid[sizex][sizey];

    if (mode == 2) {
        warn = read_grid(grid, path);
        if (warn < 0) {
            start();
        }
    } else {
        generate_grid(grid);
    }
    printf("\n1 Play\n2 Watch\n\nNow please choose your mode: ");
    scanf("%d", &mode);
    printf("\nHere is your grid:\n\n");
    print_grid(grid);
    printf("\n");

    switch (mode) {
        case 2: break;
        default: play();
    }
}

void timestamp(void)
// Stamps the time for each game in the logs //
{
    if (!player) {
        player[0] = 1;
    }
	FILE * log;
	log = fopen("Log/log.txt", "a");
	time (&btime);
	fprintf(log, "\n-------------------------------------------------------------------------------\n");
	fprintf(log, "	  %s, NEW GAME: %s", player, ctime(&btime));
	fprintf(log, "-------------------------------------------------------------------------------\n\n");
	fclose(log);
}

void generate_grid(char grid[sizex][sizey])
// Generates a grid randomly which represents like this: [1, 2, 3; 4, 5, 6] has three columns and two rows; becomes [[4, 1], [5, 2], [3, 6]]. //
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
            random = rand() % 5;
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
// Generates a grid from file which represents like this: [1, 2, 3; 4, 5, 6] has three columns and two rows; becomes [[4, 1], [5, 2], [3, 6]]. //
{
    int i, j;
    char color = 'b';
    FILE * input = fopen(path, "r");
    FILE * log = fopen("Log/log.txt", "a");

    fscanf(input, "%d%d", &j, &i);
    if (i > 36 || j > 45 || i < 5 || j < 5) {
        fclose(log);
        fclose(input);
        printf("Sorry, grid dimensions do not work.");
        return -2;
    }
    fprintf(log, "%d x %d\n", sizex, sizey);

    for (j = sizey - 1; j >= 0; j--) {
        for (i = 0; i < sizex; i++) {
            fscanf(input, "%c", &color);
            if (color != 'b' && color != 'g' && color != 'r' && color != 'y' && color != '\n' && color != ' ') {
                fclose(log);
                fclose(input);
                printf("Sorry, there is something wrong with your grid.");
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
    fprintf(log, "\n");
    fclose(log);
    fclose(input);

    return 0;
}

void print_grid(char grid[sizex][sizey])
// Prints the grid to the terminal in color //
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


// FUNCTIONS BELOW DO NOT WORK //
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

void play(void)
// Human playing mode //
{

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
	start()
}

void ai_play(void)
// just a standard greedy algorithm. could be modified. unfinished //
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
	start()
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
