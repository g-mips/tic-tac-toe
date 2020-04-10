#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define PLAYER_ONE -1
#define PLAYER_TWO -2

#define BOARD_WIDTH  17
#define BOARD_HEIGHT 9

#define BOARD_X_MIDDLE ((int)(BOARD_WIDTH / 2) + 1)
#define BOARD_Y_MIDDLE ((int)(BOARD_HEIGHT / 2) + 1)

#define NUM_COLS 3
#define NUM_ROWS 3

#define COL_SEP_POS 5
#define ROW_SEP_POS 2

static int positions[NUM_ROWS][NUM_COLS] = {
    { 0, 1, 2 },
    { 3, 4, 5 },
    { 6, 7, 8 }
};

/*
01234|6789 |
     |     |
_____|_____|_____
     |     |
     |     |
_____|_____|_____
     |     |
     |     |
     |     |
*/

char interpret_position_number(int pos_num)
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

void clear_screen()
{
    static const char ansi_clear[] = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, ansi_clear, sizeof(ansi_clear));
}

void get_term_size(int *x, int *y)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    *x = w.ws_col;
    *y = w.ws_row;
}

void setup_board()
{
    clear_screen();

    int beginning_x_pos = 0;
    int beginning_y_pos = 0;

    get_term_size(&beginning_x_pos, &beginning_y_pos);

    beginning_x_pos = (beginning_x_pos / 2) - BOARD_X_MIDDLE;
    beginning_y_pos = (beginning_y_pos / 2) - BOARD_Y_MIDDLE;

    // Get board centered downward
    for (int i = 0; i < beginning_y_pos; ++i)
    {
        puts("");
    }

    size_t cell_y_pos = 0;
    for (size_t y_pos = 0; y_pos < BOARD_HEIGHT; ++y_pos)
    {
        // Get row centered
        for (int i = 0; i < beginning_x_pos; ++i)
        {
            printf(" ");
        }

        size_t cell_x_pos = 0;
        for (size_t x_pos = 0; x_pos < BOARD_WIDTH; ++x_pos)
        {
            // Are we at a column seperator?
            if (cell_x_pos != 0 && (cell_x_pos % COL_SEP_POS) == 0)
            {
                printf("|");
                cell_x_pos = 0;
            }
            else
            {
                if (cell_y_pos == 1 && cell_x_pos == 2)
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

                    printf("%c", interpret_position_number(positions[pos_row][pos_col]));
                }
                // Are we at a row seperator
                else if (cell_y_pos != 0 && y_pos != (BOARD_HEIGHT - 1) &&
                    (cell_y_pos % ROW_SEP_POS) == 0)
                {
                    printf("_");
                }
                else
                {
                    printf(" ");
                }
                ++cell_x_pos;
            }
        }
        puts("");

        if (cell_y_pos != 0 && y_pos != (BOARD_HEIGHT - 1) &&
            (cell_y_pos % ROW_SEP_POS) == 0)
        {
            cell_y_pos = 0;
        }
        else
        {
            ++cell_y_pos;
        }
    }
}

void input_player_choice(int player_num, char *player_name)
{
    static const char valid_spot[] = "Please choose a valid spot on the board";
    int beginning_x_pos = 0;
    int beginning_y_pos = 0;

    get_term_size(&beginning_x_pos, &beginning_y_pos);

    int choosen = 0;
    while (!choosen)
    {
        printf("%s, please choose your position (0-8 from top left to "
                "bottom right): ", player_name);

        char pos_buffer[256] = "";
        (void)fgets(pos_buffer, 256, stdin);

        if (!isdigit(pos_buffer[0]) || pos_buffer[0] == '9')
        {
            printf("%s\n", valid_spot);
        }
        else
        {
            int pos = pos_buffer[0] - '0';

            int row = 0;
            if (pos >= 3)
            {
                row = (pos - 6) < 0 ? 1 : 2;
            }
            int col = pos % 3;

            if (positions[row][col] == PLAYER_ONE ||
                positions[row][col] == PLAYER_TWO)
            {
                printf("%s\n", valid_spot);
            }
            else
            {
                positions[row][col] = player_num;
                choosen = 1;
            }
        }
    }
}

int win_condition()
{
    int player_who_won = 0;
    for (size_t row = 0; row < NUM_ROWS; ++row)
    {
        if (positions[row][0] == positions[row][1] &&
            positions[row][0] == positions[row][2])
        {
            player_who_won = positions[row][0];
            break;
        }
        else if (positions[0][row] == positions[1][row] &&
                 positions[0][row] == positions[2][row])
        {
            player_who_won = positions[0][row];
            break;
        }

        if (row == 1)
        {
            if (positions[row][1] == positions[0][0] &&
                positions[row][1] == positions[2][2])
            {
                player_who_won = positions[row][1];
                break;
            }
            else if (positions[row][1] == positions[0][2] &&
                     positions[row][1] == positions[2][0])
            {
                player_who_won = positions[row][1];
                break;
            }
        }
    }

    return player_who_won;
}

void print_winner(int player_who_won, char *player_one_name, char *player_two_name)
{
    if (player_who_won == PLAYER_ONE)
    {
        printf("%s won!\n", player_one_name);
    }
    else
    {
        printf("%s won!\n", player_two_name);
    }
}

int play(char *player_one_name, char *player_two_name)
{
    int player_who_won = 0;
    int done = 0;
    while (!done)
    {
        setup_board();
        input_player_choice(PLAYER_ONE, player_one_name);
        if ((player_who_won = win_condition()) != 0)
        {
            setup_board();
            print_winner(player_who_won, player_one_name, player_two_name);
            done = 1;
        }
        else
        {
            setup_board();
            input_player_choice(PLAYER_TWO, player_two_name);
            if ((player_who_won = win_condition()) != 0)
            {
                setup_board();
                print_winner(player_who_won, player_one_name, player_two_name);
                done = 1;
            }
        }
    }

    return 0;
}

int main()
{
    clear_screen();
    char player_one_name[256] = "";
    char player_two_name[256] = "";

    printf("Please enter player one's name: ");
    fgets(player_one_name, sizeof(player_one_name), stdin);
    char *new_line = strchr(player_one_name, '\n');
    *new_line = '\0';

    printf("Please enter player two's name: ");
    fgets(player_two_name, sizeof(player_two_name), stdin);
    new_line = strchr(player_two_name, '\n');
    *new_line = '\0';

    return play(player_one_name, player_two_name);
}
