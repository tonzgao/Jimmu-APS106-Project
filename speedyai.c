//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//==========================================================================================================//
//                                                 TEAM Jimmu                                               //
//                           Zipeng Cai, Anthony Gao, Richard Shangguan, Jimmy Tieu                         //
//                            999780367   999826434       999959200       998690135                         //
//==========================================================================================================//
//                                                                                                          //
// Written for Microsoft Windows 7 and 8 in Codeblocks and Sublime Text 2. Compiled with GCC in Codeblocks. //
//                        Viewable at git://github.com/tonzgao/Jimmu-APS106-Project.git                     //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// HEADERS //
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// GLOBAL VARIABLES //
char player[25] = {'a'};                                  // for the log: player name //
time_t btime;                                       // for the log: time game starts //
int sizex = 0, sizey = 0, steps = 0,facblue,facgreen,facred,facyellow,pb=0,pr=0,pg=0,py=0;               // indicate the size of the grid, and size of each move, declares colour factors and priorities for the ai //
char color = 'b',ShineColour='b';                                  // indicates color as a hack for the expand function, arbitrarily declares current color that the ai sees as 'b' //
char dumb_grid[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E','F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'}; // to follow prescribed coordinate system //

// FUNCTIONS //
int return_int(char c)
// function which deals with the dumb coordinate system by changing characters given by the user into applicable ints //
{
    int i;
    if (c >= '0' && c <= '9') {
        i = (int)c - 48;
    } else if (c >= 'A' && c <= 'Z') {                  // A to Z represents 10 to 35 //
        i = (int)c - 55;
    } else if (c >= 'a' && c <= 'z') {
        i = (int)c - 87;
    } else if (c == '!') {                              // ! is used as a special character for program termination //
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
        return -1;                                      // if the coordinates given are empty, expand indicates failure //
    }
    if (current == mode || (mode == 1 && current == color)) {
        // Mode is usually the color we are looking for. //
        // However, when the function is called from possible, mode is 1 //
        // Then the function relies on the global variable color to tell what it is looking for. //
        altgrid[x][y] = caps(altgrid[x][y]);            // caps() the coordinate in question for collapse() to remove //
        steps++;                                        // increases the global variable steps by one if the coordinate selected is the same color as color //
        if (mode == 1 && steps > 1) {
            return 2;                                   // if called from possible() returns possible as soon as it can //
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
    return steps;                                       // global variable steps indicates how many times expand ran //
}

int possible(char grid[sizex][sizey], char altgrid[sizex][sizey])
// Returns 1 if valid moves exist, otherwise returns 0 //
{
    int i, j, pos = 0;

    for (i=0; i < sizex; i++) {
        for (j=0; j < sizey; j++) {
            // checks each element in the grid for possibleness until one is found //
            color = altgrid[i][j];
            steps = 0;
            pos = expand(altgrid, i, j, 1);             // note that mode is 1, indicating expand will end as soon as it can //
            if (pos > 1) {
                mate_grid(grid, altgrid);               // reverts changes to the grid for later use before returning //
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
                counter[i]++;                               // increases the size of the land claimed by darkness //
                tempcount++;                                // describes the gains made by darkness that turn //
                if (firstz == -1) {                         // time saver when dropping elements down //
                    firstz = j;
                }
            }
        }
        if (counter[i] > 0) {
            if (counter[i] >= sizey - 1) {
                trouble++;                                  // if an empty column exists we will have to deal with it later //
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
                altgrid[i][sizey-1] = '0';                  // land claimed by darkness is set to 0. Not sure why this works; somehow it ends up behaving as '/0'//
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
    mate_grid(altgrid, grid);                               // Updates the main grid. //
}

void generate_grid(char grid[sizex][sizey], char altgrid[sizex][sizey])
// Generates a grid pseudo-randomly. In our representation scheme, each x coordinate is a column, each y is a row. //
{
    return;
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
    fprintf(log, "\n%d %d", sizey, sizex);

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
                fprintf(log, "%c", p);
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
    return;
}


void play(char grid[sizex][sizey], char altgrid[sizex][sizey])
// Human playing mode //
{
    return;
}
void color_factor(char grid[sizex][sizey],char prior[4])
//Sets priorities for ai play to focus on eliminating colours that are less common//
{ //Colours are associated with an integer position p(colour) on the array "prior"//
    int i,j,coin;
    facred=0; //resets priorities and color occurance factors for a new grid//
    facblue=0;
    facgreen=0;
    facyellow=0;
    pb=0;
    pr=0;
    py=0;
    pg=0;

    for(j=0;j<sizey;j++){ //This loop counts the occurance of each characterv organized in colour occurance factors//
        for(i=0;i<sizex;i++){
            if(grid[i][j]=='0'){
                continue;
            }
            if(grid[i][j]=='r'){
                facred++;
            }
            else if (grid[i][j]=='g'){
                facgreen++;
            }
            else if(grid[i][j]=='y'){
                facyellow++;
            }
            else if(grid[i][j]=='b'){
                facblue++;
            }
        }
    }
//the following statements sets a value from 0 to 3 to organize the priority of elimination for each colour, 0 being the highest priority.//
    if(facblue<facgreen){
        pg++;
    }
    if(facblue<facred){
        pr++;
    }
    if(facblue<facyellow){
        py++;
    }
    if(facgreen<facblue){
        pb++;
    }
    if(facgreen<facred){
        pr++;
    }
    if(facgreen<facyellow){
        py++;
    }
    if(facyellow<facblue){
        pb++;
    }
    if(facyellow<facgreen){
        pg++;
    }
    if(facyellow<facred){
        pr++;
    }
    if(facred<facyellow){
        py++;
    }
    if(facred<facblue){
        pb++;
    }
    if(facred<facgreen){
        pg++;
    }
    if(facblue==facgreen){ //sets priority overrides for when colour factors are equal. Coin flips gives the outputs more viariety.//

        coin=rand()%2;
        if(coin==1){
            pg++;
        } else {
            pb++;
        }
    }
    if(facblue==facred){
        coin=rand()%2;
        if(coin==1){
            pr++;
        } else {
            pb++;
        }
    }
    if(facblue==facyellow){
        coin=rand()%2;
        if(coin==1){
            py++;
        } else {
            pb++;
        }
    }
    if(facgreen==facred){
        coin=rand()%2;
        if(coin==1){
            pr++;
        } else {
            pg++;
        }
    }
    if(facgreen==facyellow){
        coin=rand()%2;
        if(coin==1){
            py++;
        } else {
            pg++;
        }
    }
    if(facyellow==facred){
        coin=rand()%2;
        if(coin==1){
            pr++;
        } else {
            py++;
        }
    }
    prior[pg]='g';
    prior[pb]='b';
    prior[py]='y';
    prior[pr]='r';
}

void ai_play(char grid[sizex][sizey], char altgrid[sizex][sizey])
// ai play mode. Standard conservative brute force algorithm with a priority for picking rare colours //
{
    FILE * log = fopen("log.txt", "a");
    fprintf(log, "\nSTART");
    char e = 'a', hidden_grid[sizex][sizey],prior[4],color_facnew; //prior stores color priority values for access by the ai via prior[ccontrol], higher priority values have a lower ccontrol value.//
    int score = 0,ccontrol=0, turn, x = 1, y = 1, hx =0, hy =0,gatekeeper=1,turngate=0; //gatekeeper checks if co-ordinates for a given priority is updated, and allows the program to advance to the next priority(ccontrol) if it does not.//
    double shiny=0., best=1.;//variables used to compare the score of the current and previous co-ordinate selections//

    printf("\n");
    print_grid(grid);
    int counter[37] = {0};
    mate_grid(grid, hidden_grid);                           // creates a hidden grid, which is used for shininess tests //

    e = getchar();
    for (turn = 0; possible(grid, altgrid) == 1&&ccontrol<4;) {
        if(turngate==0){
            turn++;
            printf("\n\nTurn: %d, Current Score: %d\nPress enter for the next turn.\n", turn, score);

        }
        turngate=1;                                      // player presses enter for the next turn, or inputs ! as a hidden feature to terminate the game //
        if (e == '!') {                         //turngate allows the ai to think and switch between priority values (ccontrol) without using a turn//
            fprintf(log, "\nGAME CANCELLED\n\nFINAL SCORE: %d (AI)\n", score);
            fclose(log);
            exit(31415);
        }
        color_factor(grid,prior);
        gatekeeper=1;
        for (x = 0; x < sizex; x++) { //this is the brute force section of the ai function, it scans the grid for all co-ordinates//
            for (y = 0; y < sizey; y++) {
                if (hidden_grid[x][y] > 97) {
                    steps = 0;
                    color_facnew=prior[ccontrol];//sets the colour to be eliminated//
                    ShineColour=hidden_grid[x][y];//obtains the current color for comparison//
                    shiny = expand(hidden_grid, x, y, grid[x][y]);
                    if (ShineColour==color_facnew&&shiny > 1. && 1./shiny < 1./best) {//brute force check for combinations for priority colour, favouring combinations with smaller score.
                        best = shiny;
                        hx = x;
                        hy = y;
                        gatekeeper=0;//opens the door for the grid to be collapsed//
                        turngate=0;//demonstrates that the ai has finished thinking, consuming a turn//
                    }
                }
            }
        }
        if (gatekeeper==1){//if hx and hy are not updated, eg, there are not possible combinations for the selected color, the next rarest colour is chosen//
            ccontrol++;//advances the priority array to a lower priority colour//
            mate_grid(grid,hidden_grid);
            mate_grid(grid,altgrid);
            continue;
        }
        if (hx <= sizex && hx >= 0 && hy < sizey && hy >= 0 && grid[hx][hy] != '0') {
            steps = 0;
            steps = expand(altgrid, hx, hy, grid[hx][hy]);
            if (steps > 1) {
                collapse(altgrid, grid, counter);
                score += steps*steps;
                mate_grid(grid, altgrid);
                mate_grid(grid, hidden_grid);
                fprintf(log, "\n%d, %d, %d", hy+1, hx+1, score);
                hx = 0; hy = 0; best = 0;
                ccontrol=0;//resets the priority number for use in the new grid//
                gatekeeper=1;//closes the door for co-ordinate update checks to occur in the new grid//
                print_grid(grid);
            } else {
                printf("\n\nI feel like something is wrong here1.\n");
                fprintf(log, "\nERROR OCCURED\n");
                fclose(log);
                exit(-1);
            }
        } else {
            printf("\n\nI feel like something is wrong here.\n");
            fprintf(log, "\nERROR OCCURED\n");
            fclose(log);
            exit(-1);
        }
    }

    fprintf(log, "\nEND\n\nFINAL SCORE: %d (AI)\n", score);
    fclose(log);
    printf("\n\nAll done! The final score is %d. Now you try!\n", score);
    start();                                                // play again //
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
        for (warn = 0; sizex > 36 || sizey > 36 || sizex < 4 || sizey < 4; warn++) {
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
            if (cx[0] == '0') {                             // randomly generates a value if the player inputs 0 //
                sizex = (rand() % 15) + 9;
            } else if (cx[1] == '\n') {                     // takes letters and single digit numbers //
                sizex = return_int(cx[0]);
            } else if (cx[2] == '\n') {                     // takes two digit numbers
                sizex = 10 * (return_int(cx[0])) + return_int(cx[1]);
            } else {                                        // error
                sizex = - 1;
            }
            if (cy[0] == '0') {
                sizey = (rand() % 15) + 10;
            } else if (cy[1] == '\n') {
                sizey = return_int(cy[0]);
            } else if (cy[2] == '\n') {
                sizey = 10 * (return_int(cy[0])) + return_int(cy[1]);
            } else {
                sizey = - 1;
            }
            if (cx[0] >= 65 && sizex < 36) {                 // correctional term for letters and arrays //
                sizex++;
            }
            if (cy[0] >= 65 && sizey < 36) {
                sizey++;
            }
        }
    }

    char grid[sizex][sizey];                                // main grid which is seen by the player. Only shows finished states //
    char altgrid[sizex][sizey];                             // alternative grid used for intermediate states such as testing for possible, expanding, and collapsing //
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
        default: ai_play(grid, altgrid); break;
    }
}

int main(void)
// Opens the log file and asks the Player's name //
{
    int i;

    srand(time(NULL));                                  // seeds random number generator used throughout program //
    FILE * log;
    log = fopen("log.txt", "w");
    fprintf(log, "Zipeng Cai 999780367\nAnthony Gao 999826434\nRichard Shangguan 999959200\nJimmy Tieu 998690135\n");
    fclose(log);

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
