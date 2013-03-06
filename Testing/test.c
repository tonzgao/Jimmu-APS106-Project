// use this file to test out your new functions


#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

char player[25] = {0};
time_t btime;

void timestamp(void)
{
	FILE * log;
	log = fopen("Log/log.txt", "a");
	time (&btime);
	fprintf(log, "-------------------------------------------------------------------------------\n");
	fprintf(log, "	  %s, NEW GAME: %s", player, ctime(&btime));
	fprintf(log, "-------------------------------------------------------------------------------\n\n");
	fclose(log);
}


int main (void)
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
    scanf("%25[0-9a-zA-Z ]s", &player);
    timestamp();

    return 0;
}


