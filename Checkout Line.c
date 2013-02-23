// Zipeng Cai, Anthony Gao 999826434, Jimmy Tieu //
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

// variables: score, grid, lotsa iters, x, y coordinates, files for reading and writing;

int main(void)
{
    return 0;
}


generate_grid() //makes a "random" grid; should be playable //
read_grid() // takes a grid in from data folder to use as grid //

expand() // find which blocks need to be removed for a given coordinate in a given grid. may need several functions //
collapse() // reaarange after expanding; may not be necessary if grid is developed right //
check() // sees if moves are possible. probably just by using expand on every non edge item in list //

log() // bunch of functions to maintain log file. ideally like check can just be initiated at the end of each loop iter //

ai_play() // computer play: just random possible moves until endgame or make better if time. along with instructions for noobs? //


// current idea is that the gridso are stored in nested arrays, bottom to top, left to right. //
//[1, 2, 3; 4, 5, 6] in matlab becomes [[4, 1], [5, 2], [3, 6]]. This lets the expand and collapse functions work very well //

