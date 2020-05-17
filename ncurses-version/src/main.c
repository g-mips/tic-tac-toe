#include <stdlib.h>

#include <curses.h>

#include "player.h"
#include "board.h"

int
main(int argc, char ** argv)
{
    initscr(); /* Start curses mode */
    noecho();
    cbreak();

    refresh();
    player_get_info((LINES / 2), (COLS / 2));

    board_init();
    board_setup();

    getch();

    endwin(); /* End curses mode */

    return 0;
}
