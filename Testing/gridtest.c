#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

int sizex = 0, sizey = 0;

void generate_grid(char grid[sizex][sizey]) // you can test it in the test.c file to see if it works. I made a few edits for clarity. Also, who is paula?
// Generates a grid which represents like this: [1, 2, 3; 4, 5, 6] has three columns and two rows; becomes [[4, 1], [5, 2], [3, 6]]. //
{
    int i, j, random, seed;
    char p;

    FILE * log;
	log = fopen("Log/log.txt", "a");
	fprintf(log, "%d x %d\n", sizex, sizey);

    seed = time(NULL);
    srand(seed);
    for (j = sizey - 1; j > 0; j--) {
        for (i = 0; i < sizex; i++) {
            random = rand() %5;
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


void print_grid(char grid[sizex][sizey])
{
    int i, j;
    char color;
    for (j = sizey - 1; j > 0; j--) {
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
}

int main (void)
{
    while (sizex > 39 || sizey > 39 || sizex < 2 || sizey < 2) {
        printf("Enter size of x: ");
        scanf("%d", &sizex);
        printf("Enter size of y: ");
        scanf("%d", &sizey);

    }
    char grid[sizex][sizey];
    generate_grid(grid);
    print_grid(grid);

    return 0;
}

