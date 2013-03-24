#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

// Zipeng Cai, Anthony Gao 999826434, Richard Shangguan, Jimmy Tieu //
char player[25];
time_t btime;
int sizex = 0, sizey = 0, turnlimit = 999, steps = 0;
char color = 'b';

void mate_grid(char dominant[sizex][sizey], char recessive[sizex][sizey])
// copies one grid (dominant) into another (recessive) //
{
    int i, j;
    for (i = 0; i < sizex; i++) {
        for (j = 0; j < sizey; j++) {
            recessive[i][j] = dominant[i][j];
        }
    }
}

char caps(char c)
// puts capital version of a character. 'a' to 'A' //
{
    if (c > 97 && c < 122) {
        c = c - 'a' + 'A';
    }
    return c;
}

int expand(char altgrid[sizex][sizey], int x, int y, int mode)
// starting at a given coordinate, marks all surrounding coordinates of the same color //
{
    char current = altgrid[x][y];
    if (current == '0') {
        return -1;
    }
    if (current == mode || (mode == 1 && current == color)) {
        altgrid[x][y] = caps(altgrid[x][y]);
        steps++;
        if (mode == 1 && steps > 1) {
            return 2;
        } else {
            altgrid[x][y] = caps(altgrid[x][y]);
            if (x < sizex - 1) {
                if (altgrid[x+1][y] == current) {
                    expand(altgrid, x+1, y, mode);
                }
            }
            if (x > 0) {
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
// returns 1 if valid moves exist, otherwise returns 0 //
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
// Removes the pieces that should be removed and shuffles things down and over. Terribly ugly function, but it works! //
{
    int i, j, k, trouble = 0, firstz = -1, g = 0, tempcount = 0;

    for (i = 0; i < sizex; i++) {
        firstz = -1; g = 0; tempcount = 0;
        for (j = 0; j < sizey; j++) {
            if (altgrid[i][j] == 'B' || altgrid[i][j] == 'G' || altgrid[i][j] == 'R' || altgrid[i][j] == 'Y') {
                counter[i]++;
                tempcount++;
                if (firstz == -1) {
                    firstz = j;
                }
            }
        }
        if (counter[i] > 0) {
            if (counter[i] >= sizey - 1) {
                trouble++;
            }
            for (g = counter[i]; tempcount > 0; tempcount--) {
                for (j = 0; j < firstz; j++) {
                    if (altgrid[i][j] < 97) {
                        firstz = j;
                    }
                }
                for (k = firstz; k < sizey; k++) {
                    if (altgrid[i][k] > 97 && altgrid[i][k+1] < 97 && k < (sizey - g)) {
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

void timestamp(void)
// Stamps the time for a game in the logs //
{
    if (!player) {
        player[0] = 1;
    }
    FILE * log;
    log = fopen("Log/log.txt", "a");
    time (&btime);
    fprintf(log, "\n-------------------------------------------------------------------------------\n");
    fprintf(log, "	  NEW GAME: %s", ctime(&btime));
    fprintf(log, "-------------------------------------------------------------------------------\n\n");
    fclose(log);
}

void generate_grid(char grid[sizex][sizey], char altgrid[sizex][sizey])
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
            altgrid[i][j] = p;
            fprintf(log, "%c ", p);
        }
        fprintf(log, "\n");
    }
    fclose(log);
}

int read_grid(char grid[sizex][sizey], char altgrid[sizex][sizey], char path[25])
// Generates a grid from file which represents like this: [1, 2, 3; 4, 5, 6] has three columns and two rows; becomes [[4, 1], [5, 2], [3, 6]] //
{
    int i, j;
    char p = 'b';
    FILE * input = fopen(path, "r");
    FILE * log = fopen("Log/log.txt", "a");

    if (input == NULL) {
        fclose(log);
        fclose(input);
        printf("\nSorry, the path does not exit.\n");
        return -1;
    }

    fscanf(input, "%d%d", &j, &i);
    if (i > 36 || j > 45 || i < 6 || j < 6) {
        fclose(log);
        fclose(input);
        printf("\nSorry, grid dimensions are unreasonable.\n");
        return -2;
    }
    fprintf(log, "%d x %d", sizex, sizey);

    for (j = sizey - 1; j >= 0; j--) {
        for (i = 0; i < sizex; i++) {
            fscanf(input, "%c", &p);
            if (p != 'b' && p != 'g' && p != 'r' && p != 'y' && p != '\n' && p != ' ' && p != '0') {
                fclose(log);
                fclose(input);
                printf("\nSorry, there is something wrong with your grid.\n");
                return -3;
            }
            if (p == '\n' || p == ' ') {
                fprintf(log, "%c", p);
                i--;
            } else {
                grid[i][j] = p;
                altgrid[i][j] = p;
                fprintf(log, "%c ", p);
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
    char p;
    printf(" Y \n");
    for (j = sizey - 1; j >= 0; j--) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
        printf("%2d ", j);
        for (i = 0; i < sizex; i++) {
            p = grid[i][j];
            switch (p) {
                case 'b': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE); printf("  "); break;
                case 'r': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED); printf("  "); break;
                case 'y': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_RED); printf("  "); break;
                case 'g': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN); printf("  "); break;
                case '0': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN); printf("  "); break;
                default: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN); printf("  "); break;
            }
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
        printf("\n");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
    printf(" ");
    for (i = 0; i < sizex; i += 2) {
        printf("%4d", i);
    }
    if (i % 2 == 0) {
        printf(" ");
    }
    printf("  X");
}


void play(char grid[sizex][sizey], char altgrid[sizex][sizey])
// Human playing mode //
{
    FILE * log = fopen("Log/log.txt", "a");
    fprintf(log, "\nSTART\nx, y, score");
    int score = 0, gains = 0, turn = 1, x = 1, y = 1, warn = 0, i = 0;

    printf("\n");
    print_grid(grid);
    int counter[37] = {0};

    for (turn = 0; turn < turnlimit && possible(grid, altgrid) == 1; turn++) {
        printf("\n\nTurn: %d, Current Score: %d", turn + 1, score);
        printf("\n\nX Coordinate: ");
        scanf("%d", &x);
        printf("Y Coordinate: ");
        scanf("%d", &y);
        // crashes if given chars as input //
        if (x == 31415) {
            fprintf(log, "\nGAME CONCEEDED\n\nFINAL SCORE: %d (%s)\n", score, player);
            fclose(log);
            exit(31415);
        } else if (x <= sizex && x >= 0 && y < sizey && y >= 0 && grid[x][y] != '0') {
            steps = 0;
            steps = expand(altgrid, x, y, grid[x][y]);
            if (steps > 1) {
                collapse(altgrid, grid, counter);
                score += steps*steps;
                printf("\n");
                print_grid(grid);
                warn = 0;
            } else {
                mate_grid(grid, altgrid);
                printf("\n\nSorry, you just wasted a turn.");
                warn++;
                if (warn > 7) {
                    printf("\n\nI feel like something is wrong here.\n");
                    fprintf(log, "\nERROR OCCURED\n");
                    fclose(log);
                    exit(-1);
                }
                continue;
            }
        } else {
            printf("\nSorry, invalid coordinates.");
            turn--;
            warn++;
            if (warn > 7) {
                printf("\n\nI feel like something is wrong here.\n");
                fprintf(log, "\nERROR OCCURED\n");
                fclose(log);
                exit(-1);
            }
            continue;
        }
        fprintf(log, "\n%d, %d, %d", x+1, y+1, score);
    }


    fprintf(log, "\nEND\n\nFINAL SCORE: %d (%s)\n", score, player);
    fclose(log);
    printf("\n\nAll done! Your final score is %d.", score);
    start();
    return;
}

void ai_play(char grid[sizex][sizey], char altgrid[sizex][sizey])
// ai play mode //
{
    FILE * log = fopen("Log/log.txt", "a");
    fprintf(log, "\nSTART\nx, y, score");
    char e = 'a', hidden_grid[sizex][sizey];
    int score = 0, turn = 1, x = 1, y = 1, hx = 0, hy = 0, shiny = 0, best = 1;

    printf("\n");
    print_grid(grid);
    int counter[37] = {0};
    mate_grid(grid, hidden_grid);

    for (turn = 0; turn < turnlimit && possible(grid, altgrid) == 1; turn++) {
        printf("\n\nTurn: %d, Current Score: %d\nPress enter for the next turn.\n", turn + 1, score);
        e = getchar();
        if (e == '0') {
            fprintf(log, "\nGAME CANCELLED\n\nFINAL SCORE: %d (AI)\n", score);
            fclose(log);
            exit(31415);
        }
        for (x = 0; x < sizex; x++) {
            for (y = 0; y < sizey; y++) {
                if (hidden_grid[x][y] > 97) {
                    steps = 0;
                    shiny = expand(hidden_grid, x, y, grid[x][y]);
                    printf("%c%d ", hidden_grid[x][y], shiny);
                    if (shiny > 1 && shiny > best) {
                        best = shiny;
                        hx = x;
                        hy = y;
                    }
                }
            }
        }
        printf("%d %d%c%d", hx, hy, grid[hx][hy], best);
        if (hx <= sizex && hx >= 0 && hy < sizey && hy >= 0 && grid[hx][hy] != '0') {
            steps = 0;
            steps = expand(altgrid, hx, hy, grid[hx][hy]);
            if (steps > 1) {
                collapse(altgrid, grid, counter);
                score += steps*steps;
                printf("\n");
                print_grid(grid);
                mate_grid(grid, altgrid);
                mate_grid(grid, hidden_grid);
                hx = 0; hy = 0; best = 0;
            } else {
                printf("\n\nI feel like something is wrong here1.\n");
                fprintf(log, "\nERROR OCCURED\n");
                fclose(log);
                exit(-1);
            }
        } else {
            printf("\n\nI feel like something is wrong here2.\n");
            fprintf(log, "\nERROR OCCURED\n");
            fclose(log);
            exit(-1);
        }
        fprintf(log, "\n%d, %d, %d", x+1, y+1, score);
    }

    fprintf(log, "\nEND\n\nFINAL SCORE: %d (AI)\n", score);
    fclose(log);
    printf("\n\nAll done! The final score is %d.", score);
    start();
    return;
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
        sizex = 0; sizey = 0;
        for (warn = 0; sizex > 36 || sizey > 45 || sizex < 6 || sizey < 6; warn++) {
            if (warn > 0) {
                printf("\nSorry, please be reasonable\n");
            }
            if (warn > 7) {
                printf("\nI feel like there is a problem here.\n");
                fprintf(log, "\nERROR OCCURED\n");
                fclose(log);
                exit(-1); // giving a char for sizex or sizey causes infinite loop //
            }
            sizex = 0;
            printf("\nEnter size of x: ");
            scanf("%d", &sizex);
            if (sizex == 0) {
                int random, seed;
                seed = time(NULL);
                srand(seed);
                sizex = (rand() % 20) + 8;
            }
            sizey = 0;
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
    char altgrid[sizex][sizey];

    if (mode == 2) {
        warn = read_grid(grid, altgrid, path);
        if (warn < 0) {
            start();
        }
    } else {
        generate_grid(grid, altgrid);
    }
    printf("\n1 Play\n2 Watch\n\nNow please choose your mode: ");
    scanf("%d", &mode);
    printf("\nEnter a turn limit (0 for no limit): ");
    scanf("%d", &turnlimit);
    if (turnlimit == 0) {
        turnlimit = 9999;
    }
    fclose(log);
    switch (mode) {
        case 2: ai_play(grid, altgrid); break;
        default: play(grid, altgrid); break;
    }
}

int main(void)
// Opens the log file and asks the Player's name //
{
    FILE * log;
    log = fopen("Log/log.txt", "w");
    fprintf(log, "===============================================================================\n");
    fprintf(log, "									TEAM JIMMU\n");
    fprintf(log, "				Zipeng Cai, Anthony Gao, Richard Shangguan, Jimmy Tieu\n");
    fprintf(log, "					num		 999826434		   num				num\n");
    fprintf(log, "===============================================================================\n");
    fclose(log);

    printf("Please input your name: ");
    scanf("%[ -~]25s", &player);
    printf("\nWelcome to game! Below are all of our exciting options:\n");
    start();
    return 0;
}
