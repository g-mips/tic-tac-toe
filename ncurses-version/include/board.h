#ifndef INCLUDE_BOARD_H
#define INCLUDE_BOARD_H

#define BOARD_WIDTH  17
#define BOARD_HEIGHT 9

#define BOARD_X_MIDDLE ((int)(BOARD_WIDTH / 2) + 1)
#define BOARD_Y_MIDDLE ((int)(BOARD_HEIGHT / 2) + 1)

void board_setup(void);
void board_set_middle_y(int new_middle_y);
void board_set_middle_x(int new_middle_x);

#endif // INCLUDE_BOARD_H
