#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"

typedef struct {
    float x, y;
    int size;
    float speed;
    int hp;
    int alive;
} Enemy;

void init_enemy(Enemy *e);
void update_enemy(Enemy *e, Player *p);
void draw_enemy(Enemy e);

#endif