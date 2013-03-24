na#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h> //Just to do the upper case function

time_t btime;
int sizex = 0, sizey = 0, steps = 0;
char color = 'b';

void mate_grid(char dominant[sizex][sizey], char recessive[sizex][sizey]) //MAkes a copy of the grid
{
    int i, j;
    for (i = 0; i < sizex; i++) {
        for (j = 0; j < sizey; j++) {
            recessive[i][j] = dominant[i][j];
        }
    }
}

char caps(char c)
{
    if (c > 97 && c < 122) {
        c=toupper(c);
}
    return c;
}

int expand(char altgrid[sizex][sizey], int x, int y, int mode)
{
	char current = altgrid[x][y];
	if (current == '0')
    {
        return -1;
	}
	if (current == mode || (mode == 1 && current == color)) {
	    altgrid[x][y] = caps(altgrid[x][y]);
		steps++; //is this outside of scope?
		if (mode == 1 && steps > 1)
        {
            return 2;
		}
    else
        {
            altgrid[x][y] = caps(altgrid[x][y]);
            if (x < sizex - 1)
            {
                if (altgrid[x+1][y] == current)
                {
                    expand(altgrid, x+1, y, mode); //recursion?
                }
            }
        if (x > 0)
            {
                if (altgrid[x-1][y] == current) {
                    expand(altgrid, x-1, y, mode);
                }
            }
            if (y < sizey - 1) {
                if (altgrid[x][y+1] == current) {
                    expand(altgrid, x, y+1, mode);
                }
            }
            if (y > 0) {
                if (altgrid[x][y-1] == current) {
                    expand(altgrid, x, y-1, mode);
                }
            }
        }
	}
	return steps;
}

int possible(char grid[sizex][sizey], char altgrid[sizex][sizey])
// returns 1 if valid moves exist, otherwise returns 0
{
    int i, j, pos = 0;

    for (i=0; i < sizex; i++) {
        for (j=0; j < sizey; j++) {
            color = altgrid[i][j];
            steps = 0;
            pos = expand(altgrid, i, j, 1);
            if (pos > 1) {
                mate_grid(grid, altgrid);
                return 1;
            }
        }
    }
    mate_grid(grid, altgrid);
    return 0;
}

void collapse(char altgrid[sizex][sizey], char grid[sizex][sizey], int counter[36])

// Removes the pieces that should be removed //
{
    int i, j, k, trouble = 0, firstz = -1, g = 0, tempcount = 0;

    for (i = 0; i < sizex; i++) {
        firstz = -1; g = 0; tempcount = 0;
        for (j = 0; j < sizey; j++)
        {
            if (altgrid[i][j] == 'B' || altgrid[i][j] == 'G' || altgrid[i][j] == 'R' || altgrid[i][j] == 'Y')
                {
                counter[i]++; //maybe just do altgrid[i][j]<91&&altgrid[i][j]>64? for the first if statement
                tempcount++;
                if (firstz == -1)
                    {
                    firstz = j;
                    }
                }
        }
        if (counter[i] > 0) {
            if (counter[i] >= sizey - 1)
                {
                    trouble++;
                }
            for (g = counter[i]; tempcount > 0; tempcount--)
            {
                for (j = 0; j < firstz; j++)
                    {
                    if (altgrid[i][j] < 97)
                        {
                            firstz = j;
                        }
                    }
                for (k = firstz; k < sizey; k++)
                    {
                    if (altgrid[i][k] > 97 && altgrid[i][k+1] < 97 && k < (sizey - g))
                        {
                        continue;
                        }
                    altgrid[i][k] = altgrid[i][k+1];
                    }
                altgrid[i][sizey-1] = '0';
            }
        }
    }
    for (; trouble > 0; trouble--) {
        for (k = 0; k < sizex - 1; k++) {
            if (altgrid[k][0] == '0' && altgrid[k][1] == '0') {
                for (i = k; i < sizex; i++) {
                    for (j = 0; j < sizey; j++) {
                        altgrid[i][j] = altgrid[i+1][j];
                        altgrid[sizex][j] = '0';
                        counter[i] = counter[i+1];
                        counter[sizex] = 0;
                    }
                }
            }
        }
    }

    mate_grid(altgrid, grid);
}

void generate_grid(char grid[sizex][sizey], char altgrid[sizex][sizey])
// Generates a grid randomly which represents like this: [1, 2, 3; 4, 5, 6] has three columns and two rows; becomes [[4, 1], [5, 2], [3, 6]]. //
{
    int i, j, random, seed;
    char p = 'b';

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
            altgrid[i][j] = p;
        }
    }
}

void print_grid(char grid[sizex][sizey])
// Prints the grid to the terminal in color //
{
    int i, j;
    char p;
    for (j = sizey - 1; j >= 0; j--) {
        for (i = 0; i < sizex; i++) {
            p = grid[i][j];
            switch (p) {
                case 'b': printf("b"); break;
                case 'r': printf("r"); break;
                case 'y': printf("y"); break;
                case 'g': printf("g"); break;
                case '0': printf(" "); break;
                default: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED); printf("%c", p); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN); break;
            }
        }
        printf("\n");
    }
}

void test(void)
{
    sizex = 30; sizey = 9;
    char grid[sizex][sizey];
    char altgrid[sizex][sizey];
    int counter[37] = {0}, i = 0;
    generate_grid(grid, altgrid);
    printf("\nPossible: %d", possible(grid, altgrid));
    printf("\n\n");
    steps = 0;
    printf("expand: %d\n", expand(altgrid, 5, 5, grid[5][5]));
    printf("\nalt\n");
    print_grid(altgrid);
    if (steps > 1) {
        collapse(altgrid, grid, counter);
    } else {
        mate_grid(grid, altgrid);
    }
    printf("\n");
    print_grid(grid);
    expand(altgrid, 20, 6, grid[20][6]);
    printf("\n");
    print_grid(altgrid);
    if (steps > 1) {
        collapse(altgrid, grid, counter);
    } else {
        mate_grid(grid, altgrid);
    }
    printf("\n");
    print_grid(grid);

    scanf("%d", &i);
    if (i == 0) {
        exit(0);
    } else {
        test();
    }
}

int main(void)
{
    test();
    return 0;
}
