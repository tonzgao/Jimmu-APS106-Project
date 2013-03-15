// testing ai //

int main (void)
{
    for (i = 0; i < 100; i++) {
        score1 = ai_play1()
        score2 = ai_play2()
        total1 += score1
        total2 += score2
    }

    printf("%d%d", score1, score2)
}

int h(x)

void ai_play2() {
    score2 = score1 + score*h(x)
}

void ai_play1()
// just a standard greedy algorithm. unfinished //
{
	int warn;
	FILE * log = fopen("Log/log.txt");

	timestamp();

	int sizex = 0, sizey = 0;
    for (warn = 0; sizex > 36 || sizey > 45 || sizex < 5 || sizey < 5; warn++) {
        if (warn > 0) {
            printf("Sorry, please be reasonable\n\n");
        }
        printf("Enter size of x: ");
        scanf("%d", &sizex);
        printf("Enter size of y: ");
        scanf("%d", &sizey);
    }
	char grid[sizex][sizey];

	if (mode == 1) {
		printf("Please type the path to your grid: ")
        scanf("%25s", &path);
        FILE * input = fopen(path, "r");
        warn = read_grid(grid, path);
        fclose(input);
        if (warn < 0) {
			printf("Sorry, your path did not work. Please press 1 to try again, or 0 to generate a grid: ")
			scanf("%d", &mode);
			play(mode);
			exit(-1);
		}
	}

	generate_grid(sizex, sizey);

	fprintf(log, "%d x %d", sizex, sizey)
	fprint(file, "%s", printf_grid(grid))
	fprintf("START\nx, y, score")
	int score = 0
	while (possible() == 1) {
		printf_grid()
		printf("Current score: %d", score)
		printf("Press any key to continue")
		getchar()
		//maybe commentary to illustrate some things at certain moments?
		highest = 2
		hx = 0
		hy = 0
		steps = 0
		for e in grid {// move this section in braces to a mode in collapse which removes the highest rank. makes altgrid not need to be global.
			for ch in grid[e]:
				if (altgrid[e][ch] is not caps):
					expand(e, ch, 0)
					if steps > highest:
						highest = steps
					hx = e
					hy = ch
					steps = 0
		}
		score += collapse(hx, hy)
		fprintf(log, "%d %d %d", x, y, score)
	}

	printf("All done!/n The final score is %d./n", score)
	fprintf(log, "END\n\nFINAL SCORE: %d (AI)", score)

	printf("Okay, you try now. Press any key to play\n")
	getchar()
	start()
}
