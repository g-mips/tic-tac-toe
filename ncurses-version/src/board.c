#include <curses.h>

#include "board.h"

/**** DEFINES ****/
#define NUM_MARKERS 9

#define WIN_BEGIN_Y_POS 1
#define WIN_BEGIN_X_POS 1

#define CELL_X_SEP_POS 6
#define CELL_Y_SEP_POS 3

#define MARKER_POS_X 3
#define MARKER_POS_Y 2

#define IS_EVEN(x) ((x % 2) == 0)

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

static char *markers[NUM_MARKERS] = {
    &board[MARKER_POS_Y][MARKER_POS_X],
    &board[MARKER_POS_Y][MARKER_POS_X + CELL_X_SEP_POS],
    &board[MARKER_POS_Y][MARKER_POS_X + (CELL_X_SEP_POS * 2)],
    &board[MARKER_POS_Y + CELL_Y_SEP_POS][MARKER_POS_X],
    &board[MARKER_POS_Y + CELL_Y_SEP_POS][MARKER_POS_X + CELL_X_SEP_POS],
    &board[MARKER_POS_Y + CELL_Y_SEP_POS][MARKER_POS_X + (CELL_X_SEP_POS * 2)],
    &board[MARKER_POS_Y + (CELL_Y_SEP_POS * 2)][MARKER_POS_X],
    &board[MARKER_POS_Y + (CELL_Y_SEP_POS * 2)][MARKER_POS_X + CELL_X_SEP_POS],
    &board[MARKER_POS_Y + (CELL_Y_SEP_POS * 2)][MARKER_POS_X + (CELL_X_SEP_POS * 2)],
};

/**** INTERNAL FUNCTION PROTOTYPES ****/
#ifdef DEBUG
static void print_markers(void);

/**** FUNCTIONS ****/
static void
print_markers(void)
{
    mvprintw(0, 0, "[ ");
    for (size_t index = 0;
         index < NUM_MARKERS;
         ++index)
    {
        printw("%c ", *markers[index]);
    }
    printw("]\n");
}
#endif

int
board_who_won()
{
    int num_moves_played = 0;
    int player_who_won = 0;

    for (size_t index = 0;
         index < NUM_MARKERS;
         ++index)
    {
        if (*markers[index] != ' ')
        {
            if (index <= 2)
            {
                // Diagonals
                if (IS_EVEN(index))
                {
                    size_t divisor = (index == 0) ? 1 : index;
                    if ((*markers[index] == *markers[index + (4 / divisor)]) &&
                        (*markers[index] == *markers[index + (8 / divisor)]))
                    {
                        player_who_won = (*markers[index] == 'X') ?
                            PLAYER_ONE : PLAYER_TWO;
                        break;
                    }
                }

                // Columns
                if ((*markers[index] == *markers[index + 3]) &&
                    (*markers[index] == *markers[index + 6]))
                {
                    player_who_won = (*markers[index] == 'X') ?
                        PLAYER_ONE : PLAYER_TWO;
                    break;
                }
            }

            // Rows
            if ((index % 3) == 0)
            {
                if ((*markers[index] == *markers[index + 1]) &&
                    (*markers[index] == *markers[index + 2]))
                {
                    player_who_won = (*markers[index] == 'X') ?
                        PLAYER_ONE : PLAYER_TWO;
                    break;
                }
            }

            ++num_moves_played;
        }
    }

    if (player_who_won == 0 && num_moves_played == NUM_MARKERS)
    {
        player_who_won = DRAW;
    }

    return player_who_won;
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

#ifdef DEBUG
    print_markers();
#endif
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
