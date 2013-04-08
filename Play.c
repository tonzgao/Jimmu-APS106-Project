// Team Jimmu: Zipeng Cai, Anthony Gao 999826434, Richard Shangguan, Jimmy Tieu 998690135 //
// Written for Microsoft Windows in Codeblocks and Sublime Text 2. Compiled with GCC in Codeblocks. //
// Viewable at git://github.com/tonzgao/Jimmu-APS106-Project.git //

// HEADERS //
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

// GLOBAL VARIABLES //
char player[25];                        // for the log: player name //
time_t btime;                           // for the log: time game starts //
int sizex = 0, sizey = 0, steps = 0;    // indicate the size of the grid, and size of each move //
char color = 'b';                       // indicates color as a hack for the expand function //
char dumb_grid[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E','F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'}; // to follow your dumb coordinate system //

// FUNCTIONS //
int return_int(char c)
// function which deals with the dumb coordinate system by changing characters given by the user into applicable ints //
{
    int i;
    if (c >= '0' && c <= '9') {
        i = (int)c - 48;
    } else if (c >= 'A' && c <= 'Z') {  // A to Z represents 10 to 35 //
        i = (int)c - 55;
    } else if (c >= 'a' && c <= 'z') {
        i = (int)c - 87;
    } else if (c == '!') {              // ! is used as a special character for program termination //
        i = 31415;
    } else {
        i = -1;
    }
    return i;
}

void mate_grid(char dominant[sizex][sizey], char recessive[sizex][sizey])
// copies the elements of one grid (dominant) into another (recessive) //
{
    int i, j;
    for (i = 0; i < sizex; i++) {
        for (j = 0; j < sizey; j++) {
            recessive[i][j] = dominant[i][j];
        }
    }
}

char caps(char c)
// returns capital version of a character. 'a' to 'A' //
{
    if (c > 97 && c < 122) {
        c = c - 'a' + 'A';
    }
    return c;
}

int expand(char altgrid[sizex][sizey], int x, int y, int mode)
// recursive function that starts at a given coordinate and marks all surrounding coordinates of the same color //
{
    char current = altgrid[x][y];
    if (current == '0') {
        return -1;                              // if the coordinates given are empty, expand indicates failure //
    }
    if (current == mode || (mode == 1 && current == color)) {
        // Mode is usually the color we are looking for. //
        // However, when the function is called from possible, mode is 1 //
        // Then the function relies on the global variable color to tell what it is looking for. //
        altgrid[x][y] = caps(altgrid[x][y]);    // caps() the coordinate in question for collapse() to remove //
        steps++;                                // increases the global variable steps by one if the coordinate selected is the same color as color //
        if (mode == 1 && steps > 1) {
            return 2;                           // if called from possible() returns possible as soon as it can //
        } else {
            altgrid[x][y] = caps(altgrid[x][y]);
            // Expands all of the coordinates surrounding the current coordinate //
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
            // checks each element in the grid for possibleness until one is found //
            color = altgrid[i][j];
            steps = 0;
            pos = expand(altgrid, i, j, 1);
            if (pos > 1) {
                mate_grid(grid, altgrid);
                return 1;
            }
        }
    }
    // afterwards removes the uppercase letters from the altgrid for reuse //
    mate_grid(grid, altgrid);
    return 0;
}

void collapse(char altgrid[sizex][sizey], char grid[sizex][sizey], int counter[36])
// Removes the pieces that should be removed and shuffles things down and over. //
{
    int i, j, k, trouble = 0, firstz = -1, g = 0, tempcount = 0;

    for (i = 0; i < sizex; i++) {
        firstz = -1; g = 0; tempcount = 0;
        for (j = 0; j < sizey; j++) {
            // removes uppercase letters made by expand() //
            if (altgrid[i][j] == 'B' || altgrid[i][j] == 'G' || altgrid[i][j] == 'R' || altgrid[i][j] == 'Y') {
                counter[i]++;               // increases the size of the land claimed by darkness //
                tempcount++;                // describes the gains made by darkness that turn //
                if (firstz == -1) {         // time saver when dropping elements down //
                    firstz = j;
                }
            }
        }
        if (counter[i] > 0) {
            if (counter[i] >= sizey - 1) {
                trouble++;                  // if an empty column exists we will have to deal with it later //
            }
            for (g = counter[i]; tempcount > 0; tempcount--) {
                // For affected columns, changes each element to the one above it, up to the land claimed by darkness. //
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
                altgrid[i][sizey-1] = '0';  // land claimed by darkness is set to 0. Not sure why this works; somehow it ends up behaving as '/0'//
            }
        }
    }
    for (; trouble > 0; trouble--) {
        // shuffles columns over if there are empty columns //
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
    mate_grid(altgrid, grid);               // Updates the main grid. //
}

void timestamp(void)
// Stamps the beginning of each game started in the log //
{
    if (!player) {
        player[24] = '\0';                  // not sure if this actually does anything //
    }
    FILE * log;
    log = fopen("log.txt", "a");
    time (&btime);
    fprintf(log, "\n-------------------------------------------------------------------------------\n");
    fprintf(log, "    NEW GAME: %s", ctime(&btime));
    fprintf(log, "-------------------------------------------------------------------------------\n\n");
    fclose(log);
}

void generate_grid(char grid[sizex][sizey], char altgrid[sizex][sizey])
// Generates a grid pseudo-randomly. In our representation scheme, each x coordinate is a column, each y is a row. //
{
    int i, j, random, seed;
    char p = 'b';

    FILE * log;
    log = fopen("log.txt", "a");
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
                default: break;             // The color that the previous coordinate was has a higher chance of being generated. //
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
// Reads a grid from file. In our representation scheme, each x coordinate is a column, each y is a row. //
{
    int i, j;
    char p = 'b';
    FILE * input = fopen(path, "r");
    FILE * log = fopen("log.txt", "a");

    if (input == NULL) {
        // checks to make sure that the path exists //
        fclose(log);
        fclose(input);
        printf("\nSorry, the path does not exit.\n");
        return -1;
    }

    fscanf(input, "%d%d", &j, &i);
    if (i > 36 || j > 36 || i < 6 || j < 6) {
        // checks to make sure that grids are not too large or too small to be ridiculous //
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
                // checks to make sure that only valid characters are used //
                fclose(log);
                fclose(input);
                printf("\nSorry, there is something wrong with your grid.\n");
                return -3;
            }
            if (p == '\n' || p == ' ') {
                // allows flexibility in the logs read. Excess newlines and double spaces are excused //
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
// Prints the grid to the terminal in color. Requires windows.h //
{
    int i, j;
    char p;
    printf(" y \n");
    for (j = sizey - 1; j >= 0; j--) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
        printf("%2c ", dumb_grid[j]);           // prints y coordinate axes //
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
    for (i = 0; i < sizex; i++) {
        if (i == 0) {
            printf("%4c", dumb_grid[i]);        // prints x coordinate axes //
        } else {
            printf("%2c", dumb_grid[i]);
        }
    }
    printf("   x");
}


void play(char grid[sizex][sizey], char altgrid[sizex][sizey])
// Human playing mode //
{
    FILE * log = fopen("log.txt", "a");
    fprintf(log, "\nSTART\nx, y, score");
    int score = 0, gains = 0, turn = 1, x = 1, y = 1, warn = 0, i = 0;
    char cx[6], cy[6];

    printf("\n");
    print_grid(grid);
    int counter[37] = {0};
    // counter is used in collapse() to keep track of how many coordinates the darkness claimed //
    getchar();
    for (turn = 0; possible(grid, altgrid) == 1; turn++) {
        // keep playing if the grid is possible //
        printf("\n\nTurn: %d, Current Score: %d", turn + 1, score);
        printf("\n\nX Coordinate: ");
        fgets(cx, 5, stdin);
        printf("Y Coordinate: ");
        fgets(cy, 5, stdin);
        x = return_int(cx[0]);
        y = return_int(cy[0]);
        if (cx[1] != '\n' || cy[1] != '\n') {
            // makes sure that coordinates given are either ints from 0 to 99 or letters from a to z or A to Z //
            printf("Please remember to use one digit coordinates from 0 to Z.\n");
            if (cx[2] == '\n' && cx[0] != '0') {
                x = 10 * (return_int(cx[0])) + return_int(cx[1]);
            }
            if (cy[2] == '\n' && cy[0] != '0') {
                y = 10 * (return_int(cy[0])) + return_int(cy[1]);
            }
            for (i = 0; i < 6; i++) {
                cy[i] = '\0'; cx[i] = '\0';
            }
        }
        if (x == 31415 && y == 31415) {
            // hidden feature. closes game if player inputs ! for both x and y //
            fprintf(log, "\nGAME CONCEEDED (%s)", player);
            fclose(log);
            exit(31415);
        } else if (x <= sizex && x >= 0 && y < sizey && y >= 0 && grid[x][y] != '0') {
            // expands and collapses the grid //
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
                // if the coordinates selected are not possible, tells the player //
                printf("\nSorry, you just wasted a turn.");
                warn++;
                if (warn > 7) {
                    // general catch all for bad behaviour //
                    printf("\n\nI feel like something is wrong here.\n");
                    fprintf(log, "\nERROR OCCURED\n");
                    fclose(log);
                    exit(-1);
                }
                continue;
            }
        } else {
            // if the coordinates given are not in [0:sizex], [0:sizey] or are blank, tells the player //
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
    start(); // play again? //
    return;
}

void ai_play(char grid[sizex][sizey], char altgrid[sizex][sizey])
// ai play mode. Standard greedy algorithm //
{
    FILE * log = fopen("log.txt", "a");
    fprintf(log, "\nSTART\nx, y, score");
    char e = 'a', hidden_grid[sizex][sizey];
    int score = 0, x = 1, y = 1, turn, hx = 0, hy = 0, shiny = 0, best = 1;

    printf("\n");
    print_grid(grid);
    int counter[37] = {0};
    mate_grid(grid, hidden_grid);               // creates a hidden grid, which is used for shininess tests //

    e = getchar();
    for (turn = 0; possible(grid, altgrid) == 1;turn++) {
        printf("\n\nTurn: %d, Current Score: %d\nPress enter for the next turn.\n", turn + 1, score);
        e = getchar();                          // player presses enter for the next turn, or inputs ! as a hidden feature to terminate the game //
        if (e == '!') {
            fprintf(log, "\nGAME CANCELLED\n\nFINAL SCORE: %d (AI)\n", score);
            fclose(log);
            exit(31415);
        }
        for (x = 0; x < sizex; x++) {
            for (y = 0; y < sizey; y++) {
                if (hidden_grid[x][y] > 97) {   // each coordinate is given a shininess value based on ranking algorithms. Currently only algorithm is size //
                    steps = 0;
                    shiny = expand(hidden_grid, x, y, grid[x][y]);
                    if (shiny > 1 && shiny > best) {
                        best = shiny;
                        hx = x;
                        hy = y;
                    }
                }
            }
        }
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
                fprintf(log, "\n%d, %d, %d", hx+1, hy+1, score);
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
    }

    fprintf(log, "\nEND\n\nFINAL SCORE: %d (AI)\n", score);
    fclose(log);
    printf("\n\nAll done! The final score is %d.", score);
    start(); // play again? //
    return;
}

void start(void)
// Initializes game and allows player to choose betwen playing or watching ai. //
{
    int mode = 2, warn, i;
    char cx[6], cy[6];
    for (i = 0; i < 6; i++) {
        cx[i] = '\0'; cy[i] = '\0';
    }
    printf("\n1 Computer Generated Grid\n2 Custom Grid\n0 EXIT\n\nPlease state what you want to play: ");
    scanf("%d", &mode);
    if (mode != 1 && mode != 2) {
        exit(0);
    }

    FILE * log = fopen("log.txt", "a");
    char path[25] = {0};
    timestamp();

    if (mode == 2) {
        // If a custom grid was selected, asks for the path for read_grid() //
        printf("\nPlease type the path to your grid: ");
        scanf("%25s", &path);
        FILE * input = fopen(path, "r");
        fscanf(input, "%d%d", &sizey, &sizex);
        fclose(input);
    } else {
        // If a generated grid was selected, asks for the coordinates required. //
        printf("\nPlease input the size of your grid.\n");
        sizex = 0; sizey = 0;
        getchar();
        for (warn = 0; sizex > 36 || sizey > 36 || sizex < 6 || sizey < 6; warn++) {
            for (i = 0; i < 6; i++) {
                cx[i] = '\0'; cy[i] = '\0';
            }
            if (warn > 0) {
                printf("\nSorry, please be reasonable\n");
            }
            if (warn > 3) {
                // if an infinite loop occurs for whatever reason (i.e. player inputs a char when asked for grid size) //
                printf("\nI feel like there is a problem here.\n");
                fprintf(log, "\nERROR OCCURED\n");
                fclose(log);
                exit(-1);
            }
            sizex = 0;
            sizey = 0;
            printf("\nEnter size of x: ");
            fgets(cx, 5, stdin);
            printf("Enter size of y: ");
            fgets(cy, 5, stdin);
            if (cx[1] == '\n') {
                sizex = return_int(cx[0]);
            } else if (cx[0] == '0') {
                int random, seed;
                seed = time(NULL);
                srand(seed);
                sizex = (rand() % 20) + 8;
            } else if (cx[2] == '\n') {
                sizex = 10 * (return_int(cx[0])) + return_int(cx[1]);
            } else {
                sizex = - 1;
            }
            if (cy[1] == '\n') {
                sizey = return_int(cy[0]);
            } else if (cy[0] == '0') {
                int random, seed;
                seed = time(NULL);
                srand(seed);
                sizey = (rand() % 22) + 8;
            } else if (cy[2] == '\n') {
                sizey = 10 * (return_int(cy[0])) + return_int(cy[1]);
            } else {
                sizey = - 1;
            }
        }
    }

    char grid[sizex][sizey];                    // main grid which is seen by the player. Only shows finished states //
    char altgrid[sizex][sizey];                 // alternative grid used for intermediate states such as testing for possible, expanding, and collapsing //
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
    log = fopen("log.txt", "w");
    fprintf(log, "===============================================================================\n");
    fprintf(log, "                                  TEAM JIMMU\n");
    fprintf(log, "              Zipeng Cai, Anthony Gao, Richard Shangguan, Jimmy Tieu\n");
    fprintf(log, "                  num      999826434         num              num\n"); // please put your student numbers in //
    fprintf(log, "===============================================================================\n");
    fclose(log);

    printf("Please input your name: ");
    scanf("%[ -~]25s", &player);
    printf("\nWelcome to game! Below are all of our exciting options:\n");
    start();
    return 0;
}


// COPYRIGHT //

//   The MIT License (MIT)
//
//Copyright (c) <2013> <Zipeng Cai, Anthony Gao, Richard Shangguan, Jimmy Tieu>
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
//The software is provided "as is", without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose and noninfringement.
//In no event shall the authors or copyright holders be liable for any claim, damages or other liability, whether in an action of contract, tort or otherwise, arising from, out of or in connection with the software or the use or other dealings in the software.
