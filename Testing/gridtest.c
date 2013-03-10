#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

int sizex = 0, sizey = 0;

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

int read_grid(char grid[sizex][sizey], char path[25])
{
    int i, j;
    char color = 'b';
    FILE * input = fopen(path, "r");
	FILE * log = fopen("Log/log.txt", "a");
	fprintf(log, "%d x %d\n", sizex, sizey);

    fscanf(input, "%d%d", &j, &i);
    printf("%d%d", i, j);
    if (i != sizex || j != sizey) {
        return -2;
    }

    for (j = sizey - 1; j >= 0; j--) {
        for (i = 0; i < sizex; i++) {
            fscanf(input, "%c", &color);
            if (color != 'b' && color != 'g' && color != 'r' && color != 'y' && color != '\n' && color != ' ') {
                return -1;
            }
            if (color == '\n' || color == ' ') {
                printf("\n");
                fprintf(log, "%c", color);
                i--;
            } else {
                printf("%c", color);
                grid[i][j] = color;
                fprintf(log, "%c ", color);
            }

        }
    }
    return 0;
}


void print_grid(char grid[sizex][sizey])
// prints the current grid to terminal //
{
    int i, j;
    char color;
    for (j = sizey - 1; j >= 0; j--) {
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
    int warn, mode = 0;
    char path[15];

    printf("Choose mode: 1 play generated, 2 play premade\n");
	scanf("%d", &mode);
	switch (mode) {
        case 1: { for (warn = 0; sizex > 39 || sizey > 39 || sizex < 2 || sizey < 2; warn++) {
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
                print_grid(grid);
                break; }
        case 2: { warn = -1;
                while (warn < 0) {
                    printf("Path please: ");
                    scanf("%25s", &path);
                    FILE * input = fopen(path, "r");
                    fscanf(input, "%d %d", &sizey, &sizex);
                    char grid[sizex][sizey];
                    warn = read_grid(grid, path);
                    fclose(input);
                }
                break; }
        default: exit(0);
	}
	// print_grid(grid); not possible because it is in the scope of the switch statement... how to fix? maybe global grid with dynamic memory? //
    return 0;
}

