#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"

void init_player(Player* p) {
    p->x = 100.0;
    p->y = 300.0;
    p->size = 40.0;
    p->speed = 6.0;

    if (p->sprite == NULL) {
        p->sprite = al_load_bitmap("sprites/barco.png");
        if (!p->sprite) {
            printf(stderr, "Erro ao carregar a imagem do jogador!\n");
        }
    }
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
    if (p.sprite) {
        float largura_visual = 110.0; // Barco mais comprido na horizontal
        float altura_visual = 65.0;

        // Centraliza o desenho do barco em cima da hitbox 40x40
        float offset_x = (largura_visual - p.size) / 2.0;
        float offset_y = (altura_visual - p.size) / 2.0;

        al_draw_scaled_bitmap(
            p.sprite,
            0, 0,
            al_get_bitmap_width(p.sprite),
            al_get_bitmap_height(p.sprite),
            p.x - offset_x,
            p.y - offset_y,
            largura_visual,
            altura_visual,
            0
        );
    }
    else {
        // Fallback: se não achar a imagem, desenha o quadrado azul original
        al_draw_filled_rectangle(p.x, p.y, p.x + p.size, p.y + p.size, al_map_rgb(50, 150, 255));
    }
}

void destroy_player(Player* p) {
    if (p->sprite) {
        al_destroy_bitmap(p->sprite);
        p->sprite = NULL;
    }
}