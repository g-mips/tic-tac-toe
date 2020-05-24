#include <stdlib.h>

#include <curses.h>

#include "player.h"
#include "board.h"

#define PLAYER_MAX_STR_SIZE 20

int
main(int argc, char ** argv)
{
    initscr(); /* Start curses mode */
    curs_set(0);
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

    int winner = 0;
    int cur_player = PLAYER_ONE;
    char *cur_player_name = player_one_name;
    bool quit = false;
    while (!quit)
    {
        mvprintw((LINES / 2) + (LINES / 4), (COLS / 2),
            "%s's turn!", cur_player_name);
        bool choosen = false;
        MEVENT event = { 0 };
        while (!choosen)
        {
            event = player_input_choice(&quit);
            if (quit)
            {
                break;
            }

            if (board_adjust_position(&event))
            {
#ifdef DEBUG
                mvprintw(0, 0, "                    ");
#endif
                if (!board_spot_taken(event))
                {
                    choosen = true;
                }
            }
#ifdef DEBUG
            else
            {
                mvprintw(0, 0, "Not valid\n");
            }
#endif
        }

        if (!quit)
        {
            board_setup(&event, cur_player);

            winner = board_who_won();
            if (winner != 0)
            {
                break;
            }

            cur_player = (cur_player == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
            cur_player_name = (cur_player == PLAYER_ONE) ?
                player_one_name : player_two_name;
        }
    }

    if (!quit)
    {
        char *winner_name = (winner == PLAYER_ONE) ?
            player_one_name : player_two_name;

        if (winner == DRAW)
        {
            move((LINES / 2) + (LINES / 4), (COLS / 2));
            clrtoeol();
            printw("Draw!");
        }
        else
        {
            move((LINES / 2) + (LINES / 4), (COLS / 2));
            clrtoeol();
            printw("%s is the winner!", winner_name);
        }

        (void)getch();
    }

    board_fini();

    endwin(); /* End curses mode */

    return 0;
}
