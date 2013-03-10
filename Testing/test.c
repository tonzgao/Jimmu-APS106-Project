// use this file to test out your new functions


#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

void print_grid(char grid[8][8])
// prints the current grid to terminal //
{
    int i, j;
    char color;
    for (j = 7; j > 0; j--) {
        for (i = 0; i < 8; i++) {
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

int read_grid(char grid[8][8], char path[15])
// not sure if works //
{
    int i, j;
    char color = 'b';
    FILE * input = fopen(path, "r");

    fscanf(input, "%d%d", &i, &j);
    printf("%d%d", i, j);
    if (i != 8 || j != 8) {
        return -2;
    }

    for (j = 7; j > 0; j--) {
        for (i = 0; i < 8; i++) {
            fscanf(input, "%c", &color);
            if (color != 'b' && color != 'g' && color != 'r' && color != 'y' && color != '\n') {
                return -1;
            }
            if (color == '\n') {
                i--;
            }
            printf("%c", color);
            grid[i][j] = color;
        }
    }
    return 0;
}

int main (void)
{
    char grid[8][8], path[15] = "Data/small.txt";
    int x = read_grid(grid, path);
    print_grid(grid);
    printf("%d", x);
    return 0;
}

