#include <stdlib.h>

#include <curses.h>

#include "player.h"
#include "board.h"

int
main(int argc, char ** argv)
{
    initscr(); /* Start curses mode */
    cbreak();

    keypad(stdscr, TRUE);

    board_set_middle_y((LINES / 2) - BOARD_Y_MIDDLE);
    board_set_middle_x((COLS / 2) - BOARD_X_MIDDLE);

    refresh();
    player_get_info((LINES / 2), (COLS / 2));

    board_setup();

    getch();
    endwin(); /* End curses mode */

    return 0;
}
