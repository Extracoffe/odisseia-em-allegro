#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

typedef struct {
    float x, y;
    int size;
    float speed;
    int attack_direction;
    int is_attacking;
    int attack_timer;
} Player;

void init_player(Player *p);
void update_player(Player *p, int up, int down, int left, int right);
void draw_player(Player p);

#endif