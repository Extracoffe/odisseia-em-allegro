#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"

typedef struct {
	//variavel que define a posicao do inimigo
    float x, y;
	//variavel que define a posicao base do inimigo
	float y_base;
	//variavel que define o tamanho do inimigo
    int size;
	//variavel que define a velocidade do inimigo
    float speed;
	//variavel tipo se refere a tipo de inimigo, 0 para obstaculo, 1 para inimigo
    int tipo;
	//variavel que define o angulo da onda do inimigo
    int angulo_onda;
	float base_y;
	ALLEGRO_BITMAP* sprite;

} Enemy;

//função que carrega os sprites inimigos
void carregar_sprites_inimigos();
//função que destroi os sprites inimigos
void destruir_sprites_inimigos();
//função que inicializa o inimigo
void init_enemy(Enemy *e, int tipo);
//função que atualiza o inimigo
void update_enemy(Enemy *e);
//função que desenha o inimigo
void draw_enemy(Enemy e);
//função que verifica se houve colisão entre o player e o inimigo
int check_collision(Player p, Enemy e);

#endif