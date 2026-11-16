#include "player.h"

void init_player(Player *p) {
	// Inicializa o player com valores padrão
    p->x = 200;
    p->y = 200;
    p->size = 30;
    p->speed = 3.0;
    p->attack_direction = 1;
    p->is_attacking = 0;
    p->attack_timer = 0;
}

void update_player(Player *p, int up, int down, int left, int right) {
	// Movimento do Player
    if (up) { p->y -= p->speed; p->attack_direction = 1; }
    if (down) { p->y += p->speed; p->attack_direction = 2; }
    if (left) { p->x -= p->speed; p->attack_direction = 3; }
    if (right) { p->x += p->speed; p->attack_direction = 4; }

	// Controle do ataque do Player
    if (p->is_attacking == 1) {
        p->attack_timer--;
        if (p->attack_timer <= 0) p->is_attacking = 0;
    }
	// Limites da tela para o Player
    //limite esquerda
    if (p->x < 0) {
        p->x = 0;
    }

    //limite direita
    if (p->x + p->size > 640) {
        p->x = 640 - p->size;
    }

    //limite cima
    if (p->y < 0) {
        p->y = 0;
    }

    //limite baixo
    if (p->y + p->size > 480) {
        p->y = 480 - p->size;
    }

    /* Colisão com o inimigo
    if (enemy.x < p->x) {
        enemy.x += enemy.speed;
    }*/
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