#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include "player.h"
#include "enemy.h"

#define TOTAL_OBSTACULOS 5
#define TOTAL_INIMIGOS 5
#define TOTAL_ENTIDADES (TOTAL_OBSTACULOS + TOTAL_INIMIGOS) // 10 no total

typedef struct {
    const char* pergunta;
    const char* opcoes; // Ex: "[1] Polifemo   [2] Medusa   [3] Minotauro"
    int resposta_correta; // 1, 2 ou 3
} QuizOdisseia;

QuizOdisseia banco_quiz[] = {
    { "Qual ciclope Odisseu cegou na caverna?", "[1] Polifemo   [2] Argos   [3] Tifao", 1 },
    { "Qual deus enviou tempestades contra Odisseu?", "[1] Hades   [2] Poseidon   [3] Ares", 2 },
    { "Como Odisseu escapou do canto das sereias?", "[1] Tapou os ouvidos   [2] Amarrou-se ao mastro   [3] Fugiu a nado", 2 },
    { "Quem esperou Odisseu em Itaca por 20 anos?", "[1] Helena   [2] Circe   [3] Penelope", 3 },
    { "Qual feiticeira transformou seus homens em porcos?", "[1] Calipso   [2] Circe   [3] Atena", 2 }
};
#define TOTAL_QUIZ 5

void reiniciar_jogo(Player* player, Enemy entidades[], int* tempo_restante, ALLEGRO_TIMER* fase_timer) {
    init_player(player);
    *tempo_restante = 15;

    // Os 5 primeiros índices (0 a 4) são Obstáculos Estáticos (Amarelos)
    for (int i = 0; i < TOTAL_OBSTACULOS; i++) {
        init_enemy(&entidades[i], 0);
    }

    // Os 5 últimos índices (5 a 9) são Inimigos em Ondas (Vermelhos)
    for (int i = TOTAL_OBSTACULOS; i < TOTAL_ENTIDADES; i++) {
        init_enemy(&entidades[i], 1);
        // Espalha os inimigos na horizontal para não virem todos empilhados
        entidades[i].x = 800.0 + ((i - TOTAL_OBSTACULOS) * 180.0);
    }

    al_start_timer(fase_timer);
}

int main() {
    if (!al_init()) return -1;
    if (!al_install_keyboard()) return -1;
    if (!al_init_font_addon()) return -1;
    if (!al_init_ttf_addon()) return -1;
    if (!al_init_primitives_addon()) return -1;
    if (!al_init_image_addon()) return -1;

    srand((unsigned int)time(NULL));

    const int LARGURA_TELA = 800;
    const int ALTURA_TELA = 600;

    ALLEGRO_DISPLAY* display = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!display) return -1;

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_TIMER* fase_timer = al_create_timer(1.0);
    ALLEGRO_FONT* font = al_create_builtin_font();

    if (!event_queue || !timer || !fase_timer || !font) return -1;

    al_set_window_position(display, 150, 100);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(fase_timer));

    al_start_timer(timer);

    // 0 = Menu, 1 = Jogando, 2 = Vitória, 3 = Game Over
    int game_state = 0;
    int quiz_atual = 0;
    int inimigo_colidido = -1;
    int up = 0, down = 0, left = 0, right = 0;
    int tempo_restante = 15;

    Player player = { 0 };
    Enemy entidades[TOTAL_ENTIDADES];
    ALLEGRO_BITMAP* bg_mar = al_load_bitmap("sprites/mar.png");
    float scroll_x = 0.0;
    float velocidade_mar = 2.5;

    bool rodando = true;
    while (rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (game_state == 0) {
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE || event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    reiniciar_jogo(&player, entidades, &tempo_restante, fase_timer);
                    game_state = 1;
                }
            }
            else if (game_state == 1) {
                switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_W:
                case ALLEGRO_KEY_UP:
                    up = 1; break;
                case ALLEGRO_KEY_S:
                case ALLEGRO_KEY_DOWN:
                    down = 1; break;
                case ALLEGRO_KEY_A:
                case ALLEGRO_KEY_LEFT:
                    left = 1; break;
                case ALLEGRO_KEY_D:
                case ALLEGRO_KEY_RIGHT:
                    right = 1; break;
                }
            }
            else if (game_state == 2 || game_state == 3) {
                if (event.keyboard.keycode == ALLEGRO_KEY_R) {
                    reiniciar_jogo(&player, entidades, &tempo_restante, fase_timer);
                    game_state = 1;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_M) {
                    al_stop_timer(fase_timer);
                    game_state = 0;
                }
            }
            else if (game_state == 4) {
                int escolha = 0;
                if (event.keyboard.keycode == ALLEGRO_KEY_1 || event.keyboard.keycode == ALLEGRO_KEY_PAD_1) escolha = 1;
                if (event.keyboard.keycode == ALLEGRO_KEY_2 || event.keyboard.keycode == ALLEGRO_KEY_PAD_2) escolha = 2;
                if (event.keyboard.keycode == ALLEGRO_KEY_3 || event.keyboard.keycode == ALLEGRO_KEY_PAD_3) escolha = 3;

                if (escolha != 0) {
                    if (escolha == banco_quiz[quiz_atual].resposta_correta) {
                        if (inimigo_colidido != -1) {
                            entidades[inimigo_colidido].x = 950.0;
                        }
                        al_start_timer(fase_timer);
                        game_state = 1;
                    }
                    else {
                        game_state = 3;
                    }
                }
            }
        }

        if (event.type == ALLEGRO_EVENT_KEY_UP) {
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_UP:
                up = 0; break;
            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_DOWN:
                down = 0; break;
            case ALLEGRO_KEY_A:
            case ALLEGRO_KEY_LEFT:
                left = 0; break;
            case ALLEGRO_KEY_D:
            case ALLEGRO_KEY_RIGHT:
                right = 0; break;
            }
        }

        // Contagem regressiva da fase (15s)
        if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == fase_timer) {
            if (game_state == 1) {
                tempo_restante--;
                if (tempo_restante <= 0) {
                    al_stop_timer(fase_timer);
                    game_state = 2; // Vitória
                }
            }
        }

        // Atualização e desenho (60 FPS)
        if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == timer) {
            scroll_x -= velocidade_mar;
            if (scroll_x <= -LARGURA_TELA) {
                scroll_x = 0.0;
            }

            if (bg_mar) {
                int w = al_get_bitmap_width(bg_mar);
                int h = al_get_bitmap_height(bg_mar);
                al_draw_scaled_bitmap(bg_mar, 0, 0, w, h, scroll_x, 0, LARGURA_TELA, ALTURA_TELA, 0);
                al_draw_scaled_bitmap(bg_mar, 0, 0, w, h, scroll_x + LARGURA_TELA, 0, LARGURA_TELA, ALTURA_TELA, 0);
            }
            else {
                al_clear_to_color(al_map_rgb(20, 25, 35));
            }

            if (game_state == 0) {
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 220, ALLEGRO_ALIGN_CENTER, "A ODISSEIA: SOBREVIVENCIA");
                al_draw_text(font, al_map_rgb(200, 200, 200), LARGURA_TELA / 2, 270, ALLEGRO_ALIGN_CENTER, "Desvie dos obstaculos (amarelos) e monstros (vermelhos) por 15s!");
                al_draw_text(font, al_map_rgb(100, 255, 100), LARGURA_TELA / 2, 330, ALLEGRO_ALIGN_CENTER, "Pressione ESPACO ou ENTER para Jogar");
            }
            else if (game_state == 1) {
                update_player(&player, up, down, left, right);

                for (int i = 0; i < TOTAL_ENTIDADES; i++) {
                    update_enemy(&entidades[i]);

                    if (check_collision(player, entidades[i])) {
                        al_stop_timer(fase_timer);
                        inimigo_colidido = i;
                        quiz_atual = rand() % TOTAL_QUIZ;
                        up = down = left = right = 0; // Zera teclas presas para o barco não andar sozinho depois
                        game_state = 4; // Estado de Pergunta
                        break;
                    }
                }

                draw_player(player);
                for (int i = 0; i < TOTAL_ENTIDADES; i++) {
                    draw_enemy(entidades[i]);
                }

                al_draw_textf(font, al_map_rgb(255, 255, 255), 20, 20, 0, "Tempo: %ds", tempo_restante);
            }
            else if (game_state == 2) {
                al_draw_text(font, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 240, ALLEGRO_ALIGN_CENTER, "PARABENS! VOCE SOBREVIVEU A FASE!");
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 300, ALLEGRO_ALIGN_CENTER, "Pressione 'R' para Reiniciar ou 'M' para o Menu");
            }
            else if (game_state == 3) {
                al_draw_text(font, al_map_rgb(255, 60, 60), LARGURA_TELA / 2, 240, ALLEGRO_ALIGN_CENTER, "GAME OVER - O navio colidiu!");
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 300, ALLEGRO_ALIGN_CENTER, "Pressione 'R' para Tentar Novamente ou 'M' para o Menu");
            }
            else if (game_state == 4) {
                draw_player(player);
                for (int i = 0; i < TOTAL_ENTIDADES; i++) {
                    draw_enemy(entidades[i]);
                }
                al_draw_textf(font, al_map_rgb(255, 255, 255), 20, 20, 0, "Tempo: %ds", tempo_restante);

                al_draw_filled_rectangle(60, 190, 740, 410, al_map_rgba(10, 15, 30, 235));
                al_draw_rectangle(60, 190, 740, 410, al_map_rgb(255, 215, 0), 3.0);

                al_draw_text(font, al_map_rgb(255, 215, 0), LARGURA_TELA / 2, 220, ALLEGRO_ALIGN_CENTER, "DESAFIO DOS DEUSES! RESPONDA PARA CONTINUAR:");
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 275, ALLEGRO_ALIGN_CENTER, banco_quiz[quiz_atual].pergunta);
                al_draw_text(font, al_map_rgb(100, 255, 150), LARGURA_TELA / 2, 330, ALLEGRO_ALIGN_CENTER, banco_quiz[quiz_atual].opcoes);
                al_draw_text(font, al_map_rgb(180, 180, 180), LARGURA_TELA / 2, 380, ALLEGRO_ALIGN_CENTER, "Pressione 1, 2 ou 3 no teclado");
            }

            al_flip_display();
        }
    }

    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_timer(fase_timer);
    destroy_player(&player);
    if (bg_mar) al_destroy_bitmap(bg_mar);

    return 0;
}