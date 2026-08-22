#include "player.h"

void init_player(Player *p) {
    p->x = 200;
    p->y = 200;
    p->size = 30;
    p->speed = 3.0;
    p->attack_direction = 1;
    p->is_attacking = 0;
    p->attack_timer = 0;
}

void update_player(Player *p, int up, int down, int left, int right) {
    if (up) { p->y -= p->speed; p->attack_direction = 1; }
    if (down) { p->y += p->speed; p->attack_direction = 2; }
    if (left) { p->x -= p->speed; p->attack_direction = 3; }
    if (right) { p->x += p->speed; p->attack_direction = 4; }

    if (p->is_attacking == 1) {
        p->attack_timer--;
        if (p->attack_timer <= 0) p->is_attacking = 0;
    }
}

void draw_player(Player p) {
    // Jogador azul
    al_draw_rectangle(p.x, p.y, p.x + p.size, p.y + p.size, al_map_rgb(0, 0, 255), 3.0);

    // Ataque vermelho
    if (p.is_attacking == 1) {
        if (p.attack_direction == 1)
            al_draw_rectangle(p.x, p.y - 15, p.x + p.size, p.y, al_map_rgb(255, 0, 0), 3.0);
        else if (p.attack_direction == 2)
            al_draw_rectangle(p.x, p.y + p.size, p.x + p.size, p.y + p.size + 15, al_map_rgb(255, 0, 0), 3.0);
        else if (p.attack_direction == 3)
            al_draw_rectangle(p.x - 15, p.y, p.x, p.y + p.size, al_map_rgb(255, 0, 0), 3.0);
        else if (p.attack_direction == 4)
            al_draw_rectangle(p.x + p.size, p.y, p.x + p.size + 15, p.y + p.size, al_map_rgb(255, 0, 0), 3.0);
    }
}