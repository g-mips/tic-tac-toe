#ifndef INCLUDE_BOARD_H
#define INCLUDE_BOARD_H

#define BOARD_WIDTH  17
#define BOARD_HEIGHT 9

#define WINDOW_WIDTH  (BOARD_WIDTH + 2)
#define WINDOW_HEIGHT (BOARD_HEIGHT + 2)

#define BOARD_X_MIDDLE ((int)(BOARD_WIDTH / 2) + 1)
#define BOARD_Y_MIDDLE ((int)(BOARD_HEIGHT / 2) + 1)

void board_setup(void);
void board_init();
void board_fini();

#endif // INCLUDE_BOARD_H
