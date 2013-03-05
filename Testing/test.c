// use this file to test out your new functions


#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

char player[25] = {0};
time_t btime;
char empty = 0;

int main (void)
{
    FILE * log = fopen("log.txt", "a+"); // for some reason if the file exists nothing is added... //
    fscanf(log, "%c", &empty);
	if (empty != "/=") {
		fprintf(log, "===============================================================================\n");
		fprintf(log, "									TEAM JIMMU\n");
		fprintf(log, "				Zipeng Cai, Anthony Gao, Richard Shangguan, Jimmy Tieu\n");
		fprintf(log, "studentnums\n");
		fprintf(log, "===============================================================================\n\n");
	}

    printf("Please input your name: ");
    scanf("%25[0-9a-zA-Z ]s", &player);
    printf("Your name is: %s", player);

    time (&btime);
	fprintf(log, "-------------------------------------------------------------------------------\n");
	fprintf(log, "	  NEW GAME: %s", ctime(&btime));
	fprintf(log, "-------------------------------------------------------------------------------\n\n");

    fclose(log);
    return 0;
}


