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
	//variavel que define a quantidade de vida do inimigo
    int hp;
	//variavel que define se o inimigo esta vivo ou morto
    int alive;
	//variavel tipo se refere a tipo de inimigo, 0 para obstaculo, 1 para inimigo
    int tipo;
	//variavel que define o angulo da onda do inimigo
    int angulo_onda;

} Enemy;

//função que inicializa o inimigo
void init_enemy(Enemy *e, int tipo);
//função que atualiza o inimigo
void update_enemy(Enemy *e, Player *p);
//função que desenha o inimigo
void draw_enemy(Enemy e);
//função que verifica se houve colisão entre o player e o inimigo
bool check_collision(Player p, Enemy e);

#endif