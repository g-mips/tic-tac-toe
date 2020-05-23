#include <curses.h>

#include "board.h"

/**** DEFINES ****/
#define NUM_COLS 3
#define NUM_ROWS 3

#define COL_SEP_POS 5
#define ROW_SEP_POS 2

/**** GLOBALS ****/
static int middle_y = 0;
static int middle_x = 0;

static WINDOW *board_win = NULL;

static int positions[NUM_ROWS][NUM_COLS] =
{
    { 0, 1, 2 },
    { 3, 4, 5 },
    { 6, 7, 8 }
};

/**** INTERNAL FUNCTION PROTOTYPES ****/
static char interpret_position_number(int pos_num);

/**** FUNCTIONS ****/
static char
interpret_position_number(
    int pos_num)
{
    char pos_char = ' ';
    if (pos_num == PLAYER_ONE)
    {
        pos_char = 'X';
    }
    else if (pos_num == PLAYER_TWO)
    {
        pos_char = 'O';
    }

    return pos_char;
}

void
board_setup(MEVENT *position, int player_num)
{
    if (position != NULL)
    {
        wmouse_trafo(board_win, &(position->y), &(position->x), false);
    }

    size_t cell_y_pos = 0;
    for (size_t y_pos = 0;
         y_pos < BOARD_HEIGHT;
         ++y_pos)
    {
        wmove(board_win, y_pos + 1, 1);
        size_t cell_x_pos = 0;
        for (size_t x_pos = 0;
             x_pos < BOARD_WIDTH;
             ++x_pos)
        {
            // Are we at a column seperator?
            if ((cell_x_pos != 0) && ((cell_x_pos % COL_SEP_POS) == 0))
            {
                waddstr(board_win, "|");
                cell_x_pos = 0;
            }
            else
            {
                if ((cell_y_pos == 1) && (cell_x_pos == 2))
                {
                    // Which position are we printing?
                    size_t pos_row = 0;
                    if (y_pos == 4)
                    {
                        pos_row = 1;
                    }
                    else if (y_pos == 7)
                    {
                        pos_row = 2;
                    }

                    size_t pos_col = 0;
                    if (x_pos == 8)
                    {
                        pos_col = 1;
                    }
                    else if (x_pos == 14)
                    {
                        pos_col = 2;
                    }

                    if ((position != NULL) && (position->y == (y_pos + 1)) &&
                        (position->x == (x_pos + 1)))
                    {
                        positions[pos_row][pos_col] = player_num;
                    }

                    //addstr
                    char position_num[256] = "";
                    sprintf(position_num, "%c",
                        interpret_position_number(positions[pos_row][pos_col]));
                    waddstr(board_win, position_num);
                }
                // Are we at a row seperator
                else if (cell_y_pos != 0 && y_pos != (BOARD_HEIGHT - 1) &&
                    (cell_y_pos % ROW_SEP_POS) == 0)
                {
                    waddstr(board_win, "_");
                }
                else
                {
                    waddstr(board_win, " ");
                }
                ++cell_x_pos;
            }
        }

        if ((cell_y_pos != 0) && (y_pos != (BOARD_HEIGHT - 1)) &&
            ((cell_y_pos % ROW_SEP_POS) == 0))
        {
            cell_y_pos = 0;
        }
        else
        {
            ++cell_y_pos;
        }
    }

    wrefresh(board_win);
}

void
board_init(void)
{
    keypad(board_win, TRUE);
    middle_y = (LINES / 2) - BOARD_Y_MIDDLE;
    middle_x = (COLS / 2) - BOARD_X_MIDDLE;
    board_win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, middle_y, middle_x);

    box(board_win, 0, 0);
}

void
board_fini(void)
{
    wclear(board_win);
    wrefresh(board_win);
    delwin(board_win);
    board_win = NULL;
}
