#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "player.h"

void init_player(Player* p) {
    p->x = 100.0;
    p->y = 300.0;
    p->size = 40.0;
    p->speed = 6.0;
    p->angulo = 0.0;

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
    // Inclina -0.22 rad (~12 graus) se estiver subindo, +0.22 se estiver descendo
    float angulo_alvo = 0.0;
    if (up && !down) angulo_alvo = -0.22;
    else if (down && !up) angulo_alvo = 0.22;

    // Interpolação suave para o barco virar naturalmente
    p->angulo += (angulo_alvo - p->angulo) * 0.15;

    // Limites da tela 800x600
    float margin = 10.0;
    if (p->x < margin) p->x = margin;
    if (p->x + p->size > 800.0 - margin) p->x = 800.0 - margin - p->size;
    if (p->y < margin) p->y = margin;
    if (p->y + p->size > 600.0 - margin) p->y = 600.0 - margin - p->size;
}

void draw_player(Player p) {
    if (p.sprite) {
        double tempo = al_get_time();

        // 1. Balanço contínuo das ondas
        float flutuar_y = sin(tempo * 5.0) * 3.0;
        float balanco_onda = sin(tempo * 3.5) * 0.06;

        // 2. Pulsação sutil simulando o impulso dos remos
        float pulso_remo = 1.0 + sin(tempo * 8.0) * 0.03;

        float w = al_get_bitmap_width(p.sprite);
        float h = al_get_bitmap_height(p.sprite);

        // Mantém o mesmo tamanho visual que você já usava (ajuste se necessário)
        float visual_w = p.size * 2.2;
        float visual_h = p.size * 2.2;

        // Ponto central da hitbox do jogador (onde o sprite vai girar)
        float centro_x = p.x + (p.size / 2.0);
        float centro_y = p.y + (p.size / 2.0) + flutuar_y;

        al_draw_scaled_rotated_bitmap(
            p.sprite,
            w / 2.0, h / 2.0,               // Pivô no centro da imagem
            centro_x, centro_y,             // Posição central na tela
            (visual_w / w) * pulso_remo,    // Escala X com impulso dos remos
            (visual_h / h),                 // Escala Y
            p.angulo + balanco_onda,        // Inclinação da direção + balanço do mar
            0
        );
    }
    else {
        al_draw_filled_rectangle(p.x, p.y, p.x + p.size, p.y + p.size, al_map_rgb(50, 200, 255));
    }
}

void destroy_player(Player* p) {
    if (p->sprite) {
        al_destroy_bitmap(p->sprite);
        p->sprite = NULL;
    }
}