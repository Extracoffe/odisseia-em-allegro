#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h> //Para testar com formas geometricas.
#include <allegro5/allegro_ttf.h>

int main() {
    //Iniciar o allegro e a fonte dele.
    al_init();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon(); //Inicia o addon de primitivas.

    //Para renderizar a janela com suas dimensões, e abaixo, onde ela vai aparecer na tela.
    ALLEGRO_DISPLAY * display = al_create_display(640, 480);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_set_window_position(display, 200, 200);

    //Fonte embutida do proprio allegro e em seguida, resumidamente, um fps.
    ALLEGRO_FONT * font = al_create_builtin_font();
    ALLEGRO_TIMER * timer = al_create_timer(1.0 / 60.0);

    //Para conseguir fechar a janela clicando no x.
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    int game_state = 0; //0 = menu, 1 = jogo, 2 = game over
    float x = 300.0;
    float y = 200.0;
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    float size = 50.0; // Tamanho do retângulo.
    float speed = 10.0; // Velocidade de movimento do retângulo.

    float margin = 10.0; // Margem para evitar que o retângulo saia da tela.

    int is_attacking = 0; // Variável para controlar o estado de ataque.
    int attack_timer = 0; // Quanto tempo o ataque dura (em frames).
    int attack_direction = 1; // Direção do ataque: 0 = nenhum, 1 = cima, 2 = baixo, 3 = esquerda, 4 = direita.

    int enemy_hp = 3; // Vida do inimigo.
    int enemy_alive = 1; // Estado do inimigo: 1 = vivo, 0 = morto.
    int enemy_size = 30; // Tamanho do inimigo.

    float enemy_x = 400.0; // Posição x do inimigo.
    float enemy_y = 200.0; // Posição y do inimigo.
    float enemy_speed = 1.0; // Velocidade do inimigo.

    while (true){
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        if( event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            break;
        }

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (game_state == 0) {
                game_state = 1; // Muda para o estado do jogo.
            }

            else if (game_state == 1) {
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_W:
                        up = 1;
                        break;
                    case ALLEGRO_KEY_S:
                        down = 1;
                        break;
                    case ALLEGRO_KEY_A:
                        left = 1;
                        break;
                    case ALLEGRO_KEY_D:
                        right = 1;
                        break;
                    case ALLEGRO_KEY_SPACE:
                        if (!is_attacking) {
                        is_attacking = 1; // Inicia o ataque.
                        attack_timer = 10; // Define a duração do ataque.
                        } 
                        break; 
                }
            }
        }

        if (event.type == ALLEGRO_EVENT_KEY_UP) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_W:
                    up = 0;
                    break;
                case ALLEGRO_KEY_S:
                    down = 0;
                    break;
                case ALLEGRO_KEY_A:
                    left = 0;
                    break;
                case ALLEGRO_KEY_D:
                    right = 0;
                    break;
            }
        }
        
        if (event.type == ALLEGRO_EVENT_TIMER) {
            // Atualiza a tela apenas quando o timer dispara
            al_clear_to_color(al_map_rgb(255, 255, 255));
            if(game_state == 0) {
                al_draw_text(font, al_map_rgb(0, 0, 0), 230, 200, 0, "Isso realmente será uma Odisséia.");
            } else if (game_state == 1) {
                if (up) { y -= speed;
                   attack_direction = 1; }
                if (down) { y += speed;
                   attack_direction = 2; }
                if (left) { x -= speed;
                   attack_direction = 3; }
                if (right) { x += speed;
                   attack_direction = 4; }
                if (is_attacking == 1){
                    attack_timer--;
                    if (attack_timer <= 0) {
                        is_attacking = 0;
                    }
                }

                if (enemy_alive == 1) {
                    if (x > enemy_x) {
                        enemy_x += enemy_speed;
                    } else if (x < enemy_x) {
                        enemy_x -= enemy_speed;
                    }
                    if (y > enemy_y) {
                        enemy_y += enemy_speed;
                    } else if (y < enemy_y) {
                        enemy_y -= enemy_speed;
                    }

                    if (is_attacking == 1 && enemy_alive == 1) {
                        if (attack_direction == 1 && y - 15 < enemy_y + enemy_size && y > enemy_y && x + size > enemy_x && x < enemy_x + enemy_size) {
                            enemy_hp--;
                            enemy_y -= 40; // Empurra o inimigo para cima
                        }
                        else if (attack_direction == 2 && y + size + 15 > enemy_y && y + size < enemy_y + enemy_size && x + size > enemy_x && x < enemy_x + enemy_size) {
                            enemy_hp--;
                            enemy_y += 40; // Empurra o inimigo para baixo
                        }
                        else if (attack_direction == 3 && x - 15 < enemy_x + enemy_size && x > enemy_x && y + size > enemy_y && y < enemy_y + enemy_size) {
                            enemy_hp--;
                            enemy_x -= 40; // Empurra o inimigo para a esquerda
                        }
                        else if (attack_direction == 4 && x + size + 15 > enemy_x && x + size < enemy_x + enemy_size && y + size > enemy_y && y < enemy_y + enemy_size) {
                            enemy_hp--;
                            enemy_x += 40; // Empurra o inimigo para a direita
                        }

                        if (enemy_hp <= 0) {
                            enemy_alive = 0; // Inimigo morre
                        }
                    }

                }


                al_draw_rectangle(x, y, x + size, y + size, al_map_rgb(0, 0, 255), 3.0); // Desenha um retângulo azul

                if (enemy_alive == 1) {
                    al_draw_rectangle(enemy_x, enemy_y, enemy_x + enemy_size, enemy_y + enemy_size, al_map_rgb(0, 255, 0), 3.0); // Desenha o inimigo verde
                    al_draw_rectangle(enemy_x, enemy_y - 8, enemy_x + (enemy_hp * 10), enemy_y - 4, al_map_rgb(255, 0, 0), 2.0); // Desenha a barra de vida do inimigo
                }

                if (is_attacking == 1) {
                    if (attack_direction == 1) {
                        al_draw_rectangle(x, y - 15, x + size, y, al_map_rgb(255, 0, 0), 3.0);
                    }
                    else if (attack_direction == 2) {
                        al_draw_rectangle(x, y + size, x + size, y + size + 15, al_map_rgb(255, 0, 0), 3.0);
                    }
                    else if (attack_direction == 3) {
                        al_draw_rectangle(x - 15, y, x, y + size, al_map_rgb(255, 0, 0), 3.0);
                    }
                    else if (attack_direction == 4) {
                        al_draw_rectangle(x + size, y, x + size + 15, y + size, al_map_rgb(255, 0, 0), 3.0);
                    }   
                }               
            }
            
            al_flip_display();
        }

        if (x < margin ){
            x = margin;
        }
        if(x + size > 640 - margin){
            x = 640 - size - margin;
        }
        if (y < margin) {
            y = margin;
        }
        if (y + size > 480 - margin) {
            y = 480 - size - margin;
        }
        
        al_flip_display();
    }

    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
    
}