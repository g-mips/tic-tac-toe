#include <curses.h>

#include "player.h"

/*void
player_input_choice()
{
}*/

void
player_get_info(
    int middle_screen_y,
    int middle_screen_x,
    char *player_name, size_t player_name_size,
    unsigned int player_num)
{
    WINDOW *player_info_window = newwin(
        20, 90, (middle_screen_y / 2), (middle_screen_x / 2));
    keypad(player_info_window, TRUE);

    // Create border
    box(player_info_window, 0, 0);

    // Create prompt
    mvwprintw(player_info_window, 10, 22,
        "Please enter player %u's name: ", player_num);

    // Show these things
    echo();
    wrefresh(player_info_window);

    wgetnstr(player_info_window, player_name, player_name_size);

    // Clear the screen, show the clearing, and get rid of the window
    wclear(player_info_window);
    wrefresh(player_info_window);
    delwin(player_info_window);
}

