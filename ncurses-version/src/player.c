#include <curses.h>

#include "player.h"

void
player_get_info(
    int middle_screen_y,
    int middle_screen_x)
{
    WINDOW *player_info_window = newwin(
        20, 90, (middle_screen_y / 2), (middle_screen_x / 2));

    // Create border
    box(player_info_window, 0, 0);

    // Create prompt
    mvwprintw(player_info_window, 10, 22,
        "Please enter player one's name: ");

    // Show these things
    wrefresh(player_info_window);
    getch();

    // Clear the screen, show the clearing, and get rid of the window
    wclear(player_info_window);
    wrefresh(player_info_window);
    delwin(player_info_window);
}

