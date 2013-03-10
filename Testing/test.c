// use this file to test out your new functions


#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>



char player[25] = {0};
time_t btime;
int n;

void colortest(int n)
{
    int random, seed, i, j, prev;

    seed = time(NULL);
    srand(seed);
    for(i = 0, j = 1; i < n; i++, j++) {
        random = rand() %5;
        bad:
        switch(random) {
            case 0: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED); printf("  "); prev = 0; break;
            case 1: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN); printf("  "); prev = 1;break;
            case 2: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_RED); printf("  "); prev = 2; break;
            case 3: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE); printf("  "); prev = 3; break;
            default: random = prev; goto bad; break;
        }
        if (j > sqrt(n) - 1) {
            printf("\n");
            j = 0;
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
    scanf("%d", &n);
}

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

    printf("Square number pls (like 100): ");
    scanf("%d", &n);
    colortest(n);
    printf("%d", n);

    return 0;
}


