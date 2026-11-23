#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <allegro5/allegro_primitives.h>
#include "enemy.h"
#include "player.h"

static ALLEGRO_BITMAP* spr_obstaculos[2] = { NULL, NULL };
static ALLEGRO_BITMAP* spr_monstros[2] = { NULL, NULL };

void carregar_sprites_inimigos() {
    spr_obstaculos[0] = al_load_bitmap("sprites/rocha.png");
    spr_obstaculos[1] = al_load_bitmap("sprites/naufragio.png");
    spr_monstros[0] = al_load_bitmap("sprites/monstro.png");
    spr_monstros[1] = al_load_bitmap("sprites/sereia.png");
}

void destruir_sprites_inimigos() {
    for (int i = 0; i < 2; i++) {
        if (spr_obstaculos[i]) { al_destroy_bitmap(spr_obstaculos[i]); spr_obstaculos[i] = NULL; }
        if (spr_monstros[i]) { al_destroy_bitmap(spr_monstros[i]); spr_monstros[i] = NULL; }
    }
}

void init_enemy(Enemy* e, int tipo) {
    e->tipo = tipo;
    e->size = 35.0;
    e->angulo_onda = (float)(rand() % 628) / 100.0; // Ângulo inicial aleatório para variar as ondas

    if (e->tipo == 0) {
        // OBSTÁCULO ESTÁTICO: Nasce dentro da área jogável (longe da posição inicial do player)
        e->x = 220.0 + (rand() % 520);
        e->y = 40.0 + (rand() % 500);
        e->base_y = e->y;
        e->speed = 0.0;
        e->sprite = spr_obstaculos[rand() % 2];
    }
    else {
        // INIMIGO EM ONDAS: Nasce fora da tela à direita e atravessa fazendo ondas
        e->x = 800.0 + (rand() % 400);
        e->base_y = 80.0 + (rand() % 440);
        e->y = e->base_y;
        e->speed = 3.5 + (rand() % 20) / 10.0; // Velocidade entre 3.5 e 5.5
        e->sprite = spr_monstros[rand() % 2];
    }
}
void update_enemy(Enemy* e) {
    // Apenas o inimigo tipo 1 se move
    if (e->tipo == 1) {
        e->x -= e->speed;
        e->angulo_onda += 0.08;
        e->y = e->base_y + sin(e->angulo_onda) * 45.0; // Amplitude da onda de 45px

        // Mantém a onda dentro dos limites da tela (600px)
        if (e->y < 10.0) e->y = 10.0;
        if (e->y + e->size > 590.0) e->y = 590.0 - e->size;

        // Se sair pela esquerda, renasce na direita em uma nova altura (e troca o sprite)
        if (e->x + e->size < 0) {
            e->x = 800.0 + (rand() % 200);
            e->base_y = 80.0 + (rand() % 440);
            e->sprite = spr_monstros[rand() % 2];
        }
    }
}

void draw_enemy(Enemy e) {
    if (e.sprite) {
        double tempo = al_get_time();
        float w = al_get_bitmap_width(e.sprite);
        float h = al_get_bitmap_height(e.sprite);

        float visual = e.size * 1.6;
        float centro_x = e.x + (e.size / 2.0);
        float centro_y = e.y + (e.size / 2.0);

        if (e.tipo == 0) {
            // Obstáculos (rocha e naufrágio): balanço leve na maré
            float balanco = sin(tempo * 2.5 + e.x * 0.05) * 0.05;
            float flutuar = sin(tempo * 3.0 + e.y * 0.05) * 2.0;

            al_draw_scaled_rotated_bitmap(
                e.sprite,
                w / 2.0, h / 2.0,
                centro_x, centro_y + flutuar,
                visual / w, visual / h,
                balanco,
                0
            );
        }
        else {
            // Monstros (serpente e sereia): pulsação e inclinação nadando
            float pulso_x = 1.0 + sin(tempo * 6.0 + e.x * 0.05) * 0.05;
            float pulso_y = 1.0 - sin(tempo * 6.0 + e.x * 0.05) * 0.05;
            float inclinacao = sin(tempo * 4.0 + e.y * 0.05) * 0.12;

            al_draw_scaled_rotated_bitmap(
                e.sprite,
                w / 2.0, h / 2.0,
                centro_x, centro_y,
                (visual / w) * pulso_x,
                (visual / h) * pulso_y,
                inclinacao,
                0
            );
        }
    }
    else {
        if (e.tipo == 0) {
            // Obstáculo Estático / AMARELO
            al_draw_filled_rectangle(e.x, e.y, e.x + e.size, e.y + e.size, al_map_rgb(255, 220, 0));
        }
        else {
            // Inimigo em Ondas / VERMELHO
            al_draw_filled_rectangle(e.x, e.y, e.x + e.size, e.y + e.size, al_map_rgb(235, 50, 50));
        }
    }
}

int check_collision(Player p, Enemy e) {
    // Colisão AABB (caixa contra caixa)
    if (p.x < e.x + e.size &&
        p.x + p.size > e.x &&
        p.y < e.y + e.size &&
        p.y + p.size > e.y) {
        return 1;
    }
    return 0;
}