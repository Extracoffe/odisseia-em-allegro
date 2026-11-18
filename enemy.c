#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro_primitives.h>
#include "enemy.h"

void init_enemy(Enemy* e, int tipo) {
    e->tipo = tipo;
    e->size = 35.0;
    e->angulo_onda = (float)(rand() % 628) / 100.0; // Ângulo inicial aleatório para variar as ondas

    if (e->tipo == 0) {
        // OBSTÁCULO ESTÁTICO (Amarelo): Nasce dentro da área jogável (longe da posição inicial do player)
        e->x = 220.0 + (rand() % 520);
        e->y = 40.0 + (rand() % 500);
        e->base_y = e->y;
        e->speed = 0.0;
    }
    else {
        // INIMIGO EM ONDAS (Vermelho): Nasce fora da tela à direita e atravessa fazendo ondas
        e->x = 800.0 + (rand() % 400);
        e->base_y = 80.0 + (rand() % 440);
        e->y = e->base_y;
        e->speed = 3.5 + (rand() % 20) / 10.0; // Velocidade entre 3.5 e 5.5
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

        // Se sair pela esquerda, renasce na direita em uma nova altura
        if (e->x + e->size < 0) {
            e->x = 800.0 + (rand() % 200);
            e->base_y = 80.0 + (rand() % 440);
        }
    }
}

void draw_enemy(Enemy e) {
    if (e.tipo == 0) {
        // Obstáculo Estático / AMARELO
        al_draw_filled_rectangle(e.x, e.y, e.x + e.size, e.y + e.size, al_map_rgb(255, 220, 0));
    }
    else {
        // Inimigo em Ondas / VERMELHO
        al_draw_filled_rectangle(e.x, e.y, e.x + e.size, e.y + e.size, al_map_rgb(235, 50, 50));
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