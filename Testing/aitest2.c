// testing ai //


#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
// Zipeng Cai, Anthony Gao 999826434, Richard Shangguan, Jimmy Tieu //
time_t btime;
int sizex = 0, sizey = 0, turnlimit = 999,pb=0,pr=0,pg=0,py=0, steps = 0,overallcounter=0, high2 = 0, high1 = 0,counterscore=0,av1=0,av2=0,countav=0;
char color = 'b',ShineColour='b';
int facred=0,facblue=0,facgreen=0,facyellow=0;

//
//{




//  increases the shininess of certain positions a lot if the color is rare. //
//}
//int position_factor(sizex, sizey, x, y)
//{
//  increases the shininess of the coordinates slightly if the x is in the middle and the y is low.
//}

void color_factor(char grid[sizex][sizey])
    {
        int i,j;
        facred=0;
        facblue=0;
        facgreen=0;
        facyellow=0;
        pb=0;
        pr=0;
        py=0;
        pg=0;

        for(j=0;j<sizey;j++)
            {
                for(i=0;i<sizex;i++)
                {
        if (grid[i][j]!='0')
            printf("%c",grid[i][j]);
        else{printf(" ");}
                        if(grid[i][j]=='0')
                        {
                            continue;
                        }
                        if(grid[i][j]=='r')
                            {facred++;}
                        else if (grid[i][j]=='g')
                            {facgreen++;}
                        else if(grid[i][j]=='y')
                            {facyellow++;}
                        else if(grid[i][j]=='b')
                            {facblue++;}
                }
            printf("\n");
            }
            printf("\n");
            if(facblue<facgreen){
                pb++;
            }
            if(facblue<facred){
                pb++;
            }
            if(facblue<facyellow){
                pb++;
            }
            if(facgreen<facblue){
                pg++;
            }
            if(facgreen<facred){
                pg++;
            }
            if(facgreen<facyellow){
                pg++;
            }
            if(facyellow<facblue){
                py++;
            }
            if(facyellow<facgreen){
                py++;
            }
            if(facyellow<facred){
                py++;
            }
            if(facred<facyellow){
                pr++;
            }
            if(facred<facblue){
                pr++;
            }
            if(facred<facgreen){
                pr++;
            }
            printf("r%d g%d b%d y%d",pr,pg,pb,py);
            printf("y%d b%d g%d r%d\n",facyellow,facblue,facgreen,facred);
getchar();
            }

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

/*char caps(char c)
// puts capital version of a character. 'a' to 'A' //
{
    if (c > 97 && c < 122) {
        c = c - 'a' + 'A';
    }
    return c;
}*/

int expand(char altgrid[sizex][sizey], int x, int y, int mode)
// starting at a given coordinate, marks all surrounding coordinates of the same color //
{
    char current = altgrid[x][y];

    if (current == '0') {
        return -1;
    }

    if (current == mode || (mode == 1 && current == color)) {
        altgrid[x][y] = toupper(altgrid[x][y]);
        steps++;
        if (mode == 1 && steps > 1) {
            return 2;
        } else {
            altgrid[x][y] = toupper(altgrid[x][y]); //include typec.h
            //and use toupper, might make things faster than calling
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

void generate_grid(char grid[sizex][sizey], char altgrid[sizex][sizey])
// Generates a grid randomly which represents like this: [1, 2, 3; 4, 5, 6] has three columns and two rows; becomes [[4, 1], [5, 2], [3, 6]]. //
{
    int i, j, random;
    char p = 'b';

    FILE * log;
    log = fopen("Log/log.txt", "a");
    fprintf(log, "%d x %d\n", sizex, sizey);

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

int ai2(char grid[sizex][sizey], char altgrid[sizex][sizey])
// ai play mode. saver //
{
    FILE * log = fopen("Log/log.txt", "a");
    fprintf(log, "\nSTART\nx, y, score");
    char e = 'a', hidden_grid[sizex][sizey];
    int score = 0, turn = 1, x = 1, y = 1,oy=0,ox=0, hx = 0, hy = 0,finale=0,color_facold=0,color_facnew=sizey*sizex;
    double shiny = 0., best = 1.;

    int counter[37] = {0};
    mate_grid(grid, hidden_grid);
    for (turn = 0; turn < turnlimit && possible(grid, altgrid) == 1; turn++) {
    color_factor(grid);

        for (x = 0; x < sizex; x++) {
            for (y = 0; y < sizey; y++) {
                if (hidden_grid[x][y] > 97) {
                    steps = 0;

                    ShineColour=hidden_grid[x][y];
                        if(ShineColour=='b')
                        {
                            color_facnew=pb;
                        }


                        else if(ShineColour=='r')
                            {
                                color_facnew=pr;
                            }
                         else if(ShineColour=='g')
                                    {
                            color_facnew=pg;
                                    }
                         else if(ShineColour=='y')
                                {
                                    color_facnew=py;
                                }
                    shiny = expand(hidden_grid, x, y, grid[x][y]);
                if (color_facnew<=color_facold) {
                        best = shiny;
                        hx = x;
                        hy = y;
                        color_facold=color_facnew;
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
                mate_grid(grid, altgrid);
                mate_grid(grid, hidden_grid);
                fprintf(log, "\n%d, %d, %d", hx+1, hy+1, score);
                hx = 0; hy = 0; best = 0;
            }



            else{

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

    fprintf(log, "\nEND\n\nFINAL SCORE: %d (AI2)\n", score);
    fclose(log);
    return score;
}

int ai1(char grid[sizex][sizey], char altgrid[sizex][sizey])
// ai play mode. greedy //
{
    FILE * log = fopen("Log/log.txt", "a");
    fprintf(log, "\nSTART\nx, y, score");
    char e = 'a', hidden_grid[sizex][sizey];
    int score = 0, turn = 1, x = 1, y = 1, hx = 0, hy = 0;
    double shiny = 0., best = 1.;

    int counter[37] = {0};
    mate_grid(grid, hidden_grid);

    for (turn = 0; turn < turnlimit && possible(grid, altgrid) == 1; turn++) {
        for (x = 0; x < sizex; x++) {
            for (y = 0; y < sizey; y++) {
                if (hidden_grid[x][y] > 97) {
                    steps = 0;

                    shiny = expand(hidden_grid, x, y, grid[x][y]);

                    if (shiny > best) {

                        best =shiny;
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
                mate_grid(grid, altgrid);
                mate_grid(grid, hidden_grid);
                fprintf(log, "\n%d, %d, %d", hx+1, hy+1, score);
                hx = 0; hy = 0; best = 0;
            } else {
                printf("\n\nI feel like something is wrong here123.\n");
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

    fprintf(log, "\nEND\n\nFINAL SCORE: %d (AI1)\n", score);
    fclose(log);
    return score;
}

void part1(void)
// Player chooses betwen playing or watching ai. After each game, the player return here //
{
    int i, score1 = 0, temp = 0;
high1=0;
    sizex = 36; sizey = 36;

    for (i = 0; i < 30; i++) {
        char grid[sizex][sizey];
        char altgrid[sizex][sizey];

        generate_grid(grid, altgrid);

        temp = ai1(grid, altgrid);
        if (temp > high1) {
            high1 = temp;
        }
        score1 += temp;
    }
    av1 = score1/30;
    printf("score1: %d, av:%d, highest:%d \n", score1, av1, high1);
}

void part2(void)
// Player chooses betwen playing or watching ai. After each game, the player return here //
{
    int i,score2=0, temp = 0;
    high2=0;
    sizex = 36; sizey = 36;

    for (i = 0; i < 30; i++) {
        char grid[sizex][sizey];
        char altgrid[sizex][sizey];

        generate_grid(grid, altgrid);

        temp = ai2(grid, altgrid);
        if (temp > high2) {
            high2 = temp;
        }
        score2 += temp;
    }
    av2 = score2/30;
    printf("score2: %d, av:%d, highest:%d \n", score2, av2, high2);
}

void gogo (void)
{
    part2();
    part1();
overallcounter++;
    if(av2>av1)
    {
    countav++;
    }
    if(high2>high1)
    {
    counterscore++;
    }
    printf("\nPercentage beaten for score: %0.2f \n Percentage Beaten for Average: %0.2f",100*(float)counterscore/(float)overallcounter,100*(float)countav/(float)overallcounter);
    getchar();
    gogo();
}

int main(void)
// Opens the log file and asks the Player's name //
{
    int seed;

    FILE * log;
    log = fopen("Log/log.txt", "w");
    fprintf(log, "===============================================================================\n");
    fprintf(log, "									TEAM JIMMU\n");
    fprintf(log, "				Zipeng Cai, Anthony Gao, Richard Shangguan, Jimmy Tieu\n");
    fprintf(log, "					num		 999826434		   num				num\n");
    fprintf(log, "===============================================================================\n");
    fclose(log);

    seed = time(NULL);
    srand(seed);
    gogo();
    return 0;
}

