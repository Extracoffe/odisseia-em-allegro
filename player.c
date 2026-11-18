#include <allegro5/allegro_primitives.h>
#include "player.h"

void init_player(Player* p) {
    p->x = 100.0;
    p->y = 300.0;
    p->size = 40.0;
    p->speed = 6.0;
}

void update_player(Player* p, int up, int down, int left, int right) {
    if (up) p->y -= p->speed;
    if (down) p->y += p->speed;
    if (left) p->x -= p->speed;
    if (right) p->x += p->speed;

    // Limites da tela 800x600
    float margin = 10.0;
    if (p->x < margin) p->x = margin;
    if (p->x + p->size > 800.0 - margin) p->x = 800.0 - margin - p->size;
    if (p->y < margin) p->y = margin;
    if (p->y + p->size > 600.0 - margin) p->y = 600.0 - margin - p->size;
}

void draw_player(Player p) {
    // Navio/Jogador desenhado em azul claro
    al_draw_filled_rectangle(p.x, p.y, p.x + p.size, p.y + p.size, al_map_rgb(50, 150, 255));
}