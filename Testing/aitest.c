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

//int h(x)
//{
//    return 1;
//}

//void ai_play2()
//{
//    score2 = score1 + score*h(x)
//}

void ai_play(void)
// just a standard greedy algorithm. could be modified. unfinished //
{
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
