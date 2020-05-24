#ifndef INCLUDE_PLAYER_H
#define INCLUDE_PLAYER_H

void player_init(void);
MEVENT player_input_choice(bool *quit);
void player_get_info(int middle_screen_y, int middle_screen_x,
    char *player_name, size_t player_name_size,
    unsigned player_num);

#endif // INCLUDE_PLAYER_H
