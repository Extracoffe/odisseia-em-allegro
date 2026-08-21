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
                if (up) y -= speed;
                if (down) y += speed;
                if (left) x -= speed;
                if (right) x += speed;
                al_draw_rectangle(x, y, x + size, y + size, al_map_rgb(0, 0, 255), 3.0); // Desenha um retângulo azul
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