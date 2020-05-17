#include <curses.h>

#include "board.h"

/**** DEFINES ****/
#define NUM_COLS 3
#define NUM_ROWS 3

#define COL_SEP_POS 5
#define ROW_SEP_POS 2

#define PLAYER_ONE -1
#define PLAYER_TWO -2
#define DRAW -3

/**** GLOBALS ****/
static int middle_y = 0;
static int middle_x = 0;

static int positions[NUM_ROWS][NUM_COLS] =
{
    { 0, 1, 2 },
    { 3, 4, 5 },
    { 6, 7, 8 }
};

/**** INTERNAL FUNCTION PROTOTYPES ****/
static char interpret_position_number(int pos_num);

/**** FUNCTIONS ****/
void board_set_middle_y(int new_middle_y)
{
    middle_y = new_middle_y;
}

void board_set_middle_x(int new_middle_x)
{
    middle_x = new_middle_x;
}

static char
interpret_position_number(
    int pos_num)
{
    char pos_char = pos_num + '0';
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
board_setup(void)
{
    size_t cell_y_pos = 0;
    for (size_t y_pos = 0;
         y_pos < BOARD_HEIGHT;
         ++y_pos)
    {
        move(middle_y + y_pos, middle_x);
        size_t cell_x_pos = 0;
        for (size_t x_pos = 0;
             x_pos < BOARD_WIDTH;
             ++x_pos)
        {
            // Are we at a column seperator?
            if ((cell_x_pos != 0) && ((cell_x_pos % COL_SEP_POS) == 0))
            {
                addstr("|");
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

                    //addstr
                    char position_num[256] = "";
                    sprintf(position_num, "%c",
                        interpret_position_number(positions[pos_row][pos_col]));
                    addstr(position_num);
                }
                // Are we at a row seperator
                else if (cell_y_pos != 0 && y_pos != (BOARD_HEIGHT - 1) &&
                    (cell_y_pos % ROW_SEP_POS) == 0)
                {
                    addstr("_");
                }
                else
                {
                    addstr(" ");
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

    refresh();
}
