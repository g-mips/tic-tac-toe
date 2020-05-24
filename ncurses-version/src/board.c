#include <curses.h>

#include "board.h"

/**** DEFINES ****/
#define NUM_PER_COL_MARKERS 3
#define NUM_PER_ROW_MARKERS 3

#define WIN_BEGIN_Y_POS 1
#define WIN_BEGIN_X_POS 1

#define CELL_X_BEG 0
#define CELL_Y_BEG 0

#define CELL_X_SEP_POS 5
#define CELL_Y_SEP_POS 2

#define CELL_X_END (CELL_X_SEP_POS - 1)
#define CELL_Y_END (CELL_Y_SEP_POS - 1)

#define MARKER_POS_X 2
#define MARKER_POS_Y 1

/**** GLOBALS ****/
static int middle_y = 0;
static int middle_x = 0;

static WINDOW *board_win = NULL;

static int positions[NUM_PER_ROW_MARKERS][NUM_PER_COL_MARKERS] =
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

    size_t cell_y_pos = CELL_Y_BEG;
    for (size_t y_pos = WIN_BEGIN_Y_POS;
         y_pos <= BOARD_HEIGHT;
         ++y_pos)
    {
        wmove(board_win, y_pos, WIN_BEGIN_X_POS);
        size_t cell_x_pos = CELL_X_BEG;
        for (size_t x_pos = WIN_BEGIN_X_POS;
             x_pos <= BOARD_WIDTH;
             ++x_pos)
        {
            // Are we at a column seperator?
            if ((cell_x_pos != CELL_X_BEG) &&
                ((cell_x_pos % CELL_X_SEP_POS) == 0))
            {
                waddstr(board_win, "|");
                cell_x_pos = CELL_X_BEG;
            }
            else
            {
                if ((cell_y_pos == MARKER_POS_Y) &&
                    (cell_x_pos == MARKER_POS_X))
                {
                    // Which position are we printing?
                    size_t pos_row = 0;
                    if (y_pos == 5)
                    {
                        pos_row = 1;
                    }
                    else if (y_pos == 8)
                    {
                        pos_row = 2;
                    }

                    size_t pos_col = 0;
                    if (x_pos == 9)
                    {
                        pos_col = 1;
                    }
                    else if (x_pos == 15)
                    {
                        pos_col = 2;
                    }

                    // Do we have a marker to add?
                    if ((position != NULL) &&
                        (position->y == y_pos) &&
                        (position->x == x_pos))
                    {
#ifdef DEBUG
                        printw("POS_Y: %d\tPOS_X: %d\n", y_pos, x_pos);
                        printw("POS_ROW: %d\tPOS_COL: %d\n", pos_row, pos_col);
                        printw("PLAYER_NUM: %d\n", player_num);
#endif
                        positions[pos_row][pos_col] = player_num;
                    }

                    char position_num[256] = "";
                    sprintf(position_num, "%c",
                        interpret_position_number(positions[pos_row][pos_col]));
                    waddstr(board_win, position_num);
                }
                // Are we at a row seperator
                else if ((cell_y_pos != CELL_Y_BEG) &&
                         (y_pos != BOARD_HEIGHT) &&
                         ((cell_y_pos % CELL_Y_SEP_POS) == 0))
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

        if ((cell_y_pos != CELL_Y_BEG) &&
            (y_pos != BOARD_HEIGHT) &&
            ((cell_y_pos % CELL_Y_SEP_POS) == 0))
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
