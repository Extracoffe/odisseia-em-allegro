#include "enemy.h"

void init_enemy(Enemy *e) {
    e->x = 400;
    e->y = 200;
    e->size = 30;
    e->speed = 1.0;
    e->hp = 3;
    e->alive = 1;
}

void update_enemy(Enemy *e, Player *p) {
    if (e->alive == 0) return;

    // Perseguição
    if (p->x > e->x) e->x += e->speed;
    else if (p->x < e->x) e->x -= e->speed;

    if (p->y > e->y) e->y += e->speed;
    else if (p->y < e->y) e->y -= e->speed;

    // Colisão do ataque do Player no Inimigo
    if (p->is_attacking == 1) {
        if (p->attack_direction == 1 && p->y - 15 < e->y + e->size && p->y > e->y && p->x + p->size > e->x && p->x < e->x + e->size) {
            e->hp--;
            e->y -= 40; // Knockback
        }
        else if (p->attack_direction == 2 && p->y + p->size + 15 > e->y && p->y + p->size < e->y + e->size && p->x + p->size > e->x && p->x < e->x + e->size) {
            e->hp--;
            e->y += 40; // Knockback
        }
        else if (p->attack_direction == 3 && p->x - 15 < e->x + e->size && p->x > e->x && p->y + p->size > e->y && p->y < e->y + e->size) {
            e->hp--;
            e->x -= 40; // Knockback
        }
        else if (p->attack_direction == 4 && p->x + p->size + 15 > e->x && p->x + p->size < e->x + e->size && p->y + p->size > e->y && p->y < e->y + e->size) {
            e->hp--;
            e->x += 40; // Knockback
        }

        if (e->hp <= 0) e->alive = 0;
    }
}

void draw_enemy(Enemy e) {
    if (e.alive == 1) {
        // Inimigo verde
        al_draw_rectangle(e.x, e.y, e.x + e.size, e.y + e.size, al_map_rgb(0, 255, 0), 3.0);
        // Barra de Vida
        al_draw_rectangle(e.x, e.y - 8, e.x + (e.hp * 10), e.y - 4, al_map_rgb(255, 0, 0), 2.0);
    }
}