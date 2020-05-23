#include <stdlib.h>

#include <curses.h>

#include "player.h"
#include "board.h"

#define PLAYER_MAX_STR_SIZE 20

int
main(int argc, char ** argv)
{
    initscr(); /* Start curses mode */
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    refresh();

    player_init();

    char player_one_name[PLAYER_MAX_STR_SIZE] = "";
    char player_two_name[PLAYER_MAX_STR_SIZE] = "";
    player_get_info((LINES / 2), (COLS / 2),
        player_one_name, sizeof(player_one_name), 1);
    player_get_info((LINES / 2), (COLS / 2),
        player_two_name, sizeof(player_two_name), 2);

    noecho();

    board_init();
    board_setup(NULL, DRAW);

    MEVENT event = player_input_choice();

    struct coordinates position = {
        .x = event.x,
        .y = event.y
    };
    board_setup(&position, PLAYER_ONE);

    getch();

    endwin(); /* End curses mode */

    return 0;
}
