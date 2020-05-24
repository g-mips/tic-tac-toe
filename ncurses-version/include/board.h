#ifndef INCLUDE_BOARD_H
#define INCLUDE_BOARD_H

#define BOARD_WIDTH  17
#define BOARD_HEIGHT 9

#define WINDOW_WIDTH  (BOARD_WIDTH + 2)
#define WINDOW_HEIGHT (BOARD_HEIGHT + 2)

#define BOARD_X_MIDDLE ((int)(BOARD_WIDTH / 2) + 1)
#define BOARD_Y_MIDDLE ((int)(BOARD_HEIGHT / 2) + 1)

#define PLAYER_ONE -1
#define PLAYER_TWO -2
#define DRAW -3

bool board_adjust_position(MEVENT *position);
bool board_spot_taken(MEVENT position);
void board_setup(MEVENT *position, int player_num);
void board_init(void);
void board_fini(void);

#endif // INCLUDE_BOARD_H
