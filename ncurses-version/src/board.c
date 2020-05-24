#include <curses.h>

#include "board.h"

/**** DEFINES ****/
#define WIN_BEGIN_Y_POS 1
#define WIN_BEGIN_X_POS 1

#define CELL_X_SEP_POS 6
#define CELL_Y_SEP_POS 3

#define MARKER_POS_X 3
#define MARKER_POS_Y 2

/**** GLOBALS ****/
static int middle_y = 0;
static int middle_x = 0;

static WINDOW *board_win = NULL;

static char board[WINDOW_HEIGHT + 1][WINDOW_WIDTH + 1] = {
    "",
    "      |     |      ",
    "      |     |      ",
    " _____|_____|_____ ",
    "      |     |      ",
    "      |     |      ",
    " _____|_____|_____ ",
    "      |     |      ",
    "      |     |      ",
    "      |     |      ",
    ""
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

bool
board_adjust_position(MEVENT *position)
{
    bool valid = false;
    if (position != NULL)
    {
#ifdef DEBUG
        int row = 0;
        int col = 0;
        getmaxyx(stdscr, row, col);
        wmove(stdscr, row - 5, col - col);
        wprintw(stdscr, "BEF COORDS: %x %x\n", position->y, position->x);
        wmove(stdscr, row, col - col);
#endif
        valid = wmouse_trafo(board_win, &(position->y), &(position->x), false);
#ifdef DEBUG
        wprintw(stdscr, "AFT COORDS: %x %x\n", position->y, position->x);
#endif
        if (valid)
        {
            if (position->x < CELL_X_SEP_POS)
            {
                position->x = MARKER_POS_X * 1;
            }
            else if (position->x < (CELL_X_SEP_POS * 2))
            {
                position->x = MARKER_POS_X * 3;
            }
            else if (position->x < (CELL_X_SEP_POS * 3))
            {
                position->x = MARKER_POS_X * 5;
            }
            else
            {
                valid = false;
            }

            if (valid)
            {
                if (position->y <= CELL_Y_SEP_POS)
                {
                    position->y = (MARKER_POS_Y * 1) + 0;
                }
                else if (position->y <= (CELL_Y_SEP_POS * 2))
                {
                    position->y = (MARKER_POS_Y * 2) + 1;
                }
                else if (position->y <= (CELL_Y_SEP_POS * 3))
                {
                    position->y = (MARKER_POS_Y * 3) + 2;
                }
                else
                {
                    valid = false;
                }
            }
        }

#ifdef DEBUG
        wprintw(stdscr, "AFT AFT COORDS: %x %x\n", position->y, position->x);
#endif
    }

    return valid;
}

bool
board_spot_taken(MEVENT position)
{
    bool taken = false;
    if (board[position.y][position.x] != ' ')
    {
        taken = true;
    }

    return taken;
}

void
board_setup(MEVENT *position, int player_num)
{
    if (position != NULL)
    {
        board[position->y][position->x] = (player_num == PLAYER_ONE) ?
            'X' : 'O';
    }

    for (size_t y_pos = WIN_BEGIN_Y_POS;
         y_pos <= BOARD_HEIGHT;
         ++y_pos)
    {
        mvwaddnstr(
            board_win,
            y_pos, WIN_BEGIN_X_POS,
            board[y_pos] + WIN_BEGIN_X_POS, BOARD_WIDTH);
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
