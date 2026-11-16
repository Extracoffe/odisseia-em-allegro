#include "enemy.h"

void init_enemy(Enemy* e, int tipo) {
	// Inicializa o inimigo com valores padrão
    e->x = 400;
    e->y = 200;
    e->size = 30;
    e->speed = 2.0;
    e->hp = 3;
    e->tipo = tipo; 
    e->angulo_onda = 0.0;
    e->alive = 1;

    // Se for obstáculo
	if (tipo == 0) { 
		e->y = 100 + (rand() % 400); //altura aleatória para obstáculo
		e->size = 25; //tamanho do obstáculo
    }
    else {
		// Se for inimigo
		e->y = 120 + (rand() % 360); //altura aleatória para inimigo
		e->size = 20; //tamanho do inimigo
    }

}

void update_enemy(Enemy* e, Player* p) {
    if (e->alive == 0) return;

	// Movimento do inimigo/obstáculo
    if(e->tipo == 0){
		//obstáculo se move da direita para a esquerda
		e->x -= e->speed;

		// Se o obstáculo sair da tela, reposiciona ele do lado direito com uma altura aleatória
		if (e->x < -50) {
			e->x = 800 + 50;
			e->y = 100 + (rand() % 400);
		}
    }
    else {
        //inimigo move para esquerda fazendo ondas com o seno
		e->x -= (e->speed + 1.0); 
		e->angulo_onda += 0.08;
		e->y = e->y_base + sin(e->angulo_onda) * 50.0; //onda vertical

        if(e->x < -50) {
            e->x = 800 +100;
            e->y = 120 + (rand() % 360);
			e->y_base = e->y; // Atualiza a posição base para a nova altura 
        }
    }

	/* Perseguição e Colisão com o Player
    // Movimento no Eixo X
    if (p->x > e->x) {
        e->x += e->speed;
        // Se colidir indo para a direita, gruda na esquerda do player
        if (e->x + e->size > p->x && e->x < p->x + p->size && e->y + e->size > p->y && e->y < p->y + p->size) {
            e->x = p->x - e->size;
        }
    }
    else if (p->x < e->x) {
        e->x -= e->speed;
        // Se colidir indo para a esquerda, gruda na direita do player
        if (e->x + e->size > p->x && e->x < p->x + p->size && e->y + e->size > p->y && e->y < p->y + p->size) {
            e->x = p->x + p->size;
        }
    }

    // Movimento no Eixo Y
    if (p->y > e->y) {
        e->y += e->speed;
        // Se colidir indo para baixo, gruda no topo do player
        if (e->x + e->size > p->x && e->x < p->x + p->size && e->y + e->size > p->y && e->y < p->y + p->size) {
            e->y = p->y - e->size;
        }
    }
    else if (p->y < e->y) {
        e->y -= e->speed;
        // Se colidir indo para cima, gruda na base do player
        if (e->x + e->size > p->x && e->x < p->x + p->size && e->y + e->size > p->y && e->y < p->y + p->size) {
            e->y = p->y + p->size;
        }
    }

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
    }*/
}

bool check_collision(Player p, Enemy e) {
    if (e.alive == 0) return false;

	// Verifica se há colisão entre o player e o inimigo
	return (p.x < e.x + e.size &&
		p.x + p.size > e.x &&
		p.y < e.y + e.size &&
		p.y + p.size > e.y);
}

void draw_enemy(Enemy e) {
    if (e.alive == 1) {
        if (e.tipo == 0) {
			al_draw_filled_rectangle(e.x, e.y, e.x + e.size, e.y + e.size, al_map_rgb(110, 110, 115)); // Obstáculo
        }
        else {
			al_draw_filled_rectangle(e.x, e.y, e.x + e.size, e.y + e.size, al_map_rgb(180, 40, 80)); // Inimigo)
        }
    }
}

/*void draw_enemy(Enemy e) {
    if (e.alive == 1) {
        // Inimigo verde
        al_draw_rectangle(e.x, e.y, e.x + e.size, e.y + e.size, al_map_rgb(0, 255, 0), 3.0);
        // Barra de Vida
        al_draw_rectangle(e.x, e.y - 8, e.x + (e.hp * 10), e.y - 4, al_map_rgb(255, 0, 0), 2.0);
    }
}*/
