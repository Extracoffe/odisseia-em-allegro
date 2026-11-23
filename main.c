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

#define TOTAL_FASES 5
#define MAX_ENTIDADES 20 // Capacidade máxima (até 10 obstáculos + 10 monstros)

typedef struct {
    const char* pergunta;
    const char* opcoes;
    int resposta_correta; // 1, 2 ou 3
} QuizOdisseia;

QuizOdisseia banco_quiz[] = {
    { "Qual ciclope Odisseu cegou na caverna?", "[1] Polifemo   [2] Argos   [3] Tifao", 1 },
    { "Qual deus enviou tempestades contra Odisseu?", "[1] Hades   [2] Poseidon   [3] Ares", 2 },
    { "Como Odisseu escapou do canto das sereias?", "[1] Tapou os ouvidos   [2] Amarrou-se ao mastro   [3] Fugiu a nado", 2 },
    { "Quem esperou Odisseu em Itaca por 20 anos?", "[1] Helena   [2] Circe   [3] Penelope", 3 },
    { "Qual feiticeira transformou seus homens em porcos?", "[1] Calipso   [2] Circe   [3] Atena", 2 },
    { "Qual era o nome falso que Odisseu deu a Polifemo?", "[1] Ninguem   [2] Ulisses   [3] Aquiles", 1 },
    { "Qual deusa ajudou e protegeu Odisseu na jornada?", "[1] Hera   [2] Afrodite   [3] Atena", 3 },
    { "Qual monstro marinho tinha seis cabecas?", "[1] Caribdis   [2] Cila   [3] Hidra", 2 },
    { "Qual monstro criava um redemoinho mortal no mar?", "[1] Caribdis   [2] Cila   [3] Medusa", 1 },
    { "Como se chamava o filho de Odisseu e Penelope?", "[1] Telemaco   [2] Heitor   [3] Paris", 1 },
    { "Qual o nome do reino governado por Odisseu?", "[1] Esparta   [2] Itaca   [3] Micenas", 2 },
    { "Quanto tempo durou a Guerra de Troia antes da viagem?", "[1] 5 anos   [2] 10 anos   [3] 20 anos", 2 },
    { "Quem era o pai do ciclope Polifemo?", "[1] Zeus   [2] Cronos   [3] Poseidon", 3 },
    { "O que Penelope tecia e desmanchava toda noite?", "[1] Uma mortalha   [2] Uma vela de barco   [3] Um manto real", 1 },
    { "Qual planta fazia os marinheiros esquecerem de voltar?", "[1] Ambrosia   [2] Flor de Lotus   [3] Cicuta", 2 },
    { "Quem entregou um saco com todos os ventos a Odisseu?", "[1] Hermes   [2] Eolo   [3] Apolo", 2 },
    { "Por que o saco dos ventos foi aberto antes da hora?", "[1] Curiosidade e ganancia   [2] Ataque de sereias   [3] Ordem de Zeus", 1 },
    { "Qual ninfa prendeu Odisseu em sua ilha por 7 anos?", "[1] Calipso   [2] Dafne   [3] Eco", 1 },
    { "Qual deus mensageiro deu a erva moli contra Circe?", "[1] Apolo   [2] Hermes   [3] Hefesto", 2 },
    { "Quem Odisseu foi consultar no Submundo (Hades)?", "[1] Agamenon   [2] Tiresias   [3] Aquiles", 2 },
    { "Qual foi a prova final de Penelope para os pretendentes?", "[1] Luta de espadas   [2] Atirar com o arco   [3] Corrida de barcos", 2 },
    { "Quem foi o unico a reconhecer Odisseu disfarcado de mendigo?", "[1] Seu cao Argos   [2] Um pretendente   [3] O rei Menelau", 1 },
    { "Como Odisseu e seus homens fugiram da caverna de Polifemo?", "[1] Cavando um tunel   [2] Presos sob ovelhas   [3] Invisiveis", 2 },
    { "Qual povo acolheu Odisseu e o levou de volta a Itaca?", "[1] Feacios   [2] Troianos   [3] Cicones", 1 },
    { "O que os marinheiros comeram na ilha do deus Helios?", "[1] Frutos proibidos   [2] O gado sagrado   [3] Peixes dourados", 2 },
    { "Quem era o pai de Odisseu?", "[1] Laertes   [2] Priamo   [3] Nestor", 1 },
    { "Qual estrategia de Odisseu venceu a Guerra de Troia?", "[1] Fogo grego   [2] Cavalo de madeira   [3] Cerco naval", 2 },
    { "Quem eram os gigantes canibais que destruiram 11 navios?", "[1] Lestrigoes   [2] Centauros   [3] Titas", 1 },
    { "Qual princesa feacia encontrou Odisseu na praia?", "[1] Nausicaa   [2] Andromaca   [3] Cassandra", 1 },
    { "Quem escreveu o poema epico A Odisseia?", "[1] Sofocles   [2] Homero   [3] Hesiodo", 2 }
};
#define TOTAL_QUIZ 30

const char* nomes_fases[TOTAL_FASES] = {
    "Fase 1: Mar de Troia",
    "Fase 2: Ilha dos Ciclopes",
    "Fase 3: Canto das Sereias",
    "Fase 4: Cila e Caribdis",
    "Fase 5: A Furia de Poseidon"
};

int ordem_quiz[TOTAL_QUIZ];
int indice_fila_quiz = 0;

void embaralhar_quiz() {
    for (int i = 0; i < TOTAL_QUIZ; i++) {
        ordem_quiz[i] = i;
    }
    for (int i = TOTAL_QUIZ - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = ordem_quiz[i];
        ordem_quiz[i] = ordem_quiz[j];
        ordem_quiz[j] = temp;
    }
    indice_fila_quiz = 0;
}

// Configura os inimigos, tempo e velocidade de acordo com a fase (1 a 5)
void preparar_fase(Player* player, Enemy entidades[], int fase, int* total_ativos, int* tempo_restante, float* velocidade_mar, ALLEGRO_TIMER* fase_timer) {
    init_player(player);

    // Tempo cresce 5s por fase: 15s, 20s, 25s, 30s, 35s
    *tempo_restante = 10 + (fase * 5);

    // Correnteza do mar acelera a cada fase
    *velocidade_mar = 2.5 + (fase - 1) * 0.6;

    // Quantidade de cada tipo sobe de 3 (Fase 1) até 7 (Fase 5)
    int qtd_obstaculos = 2 + fase;
    int qtd_inimigos = 2 + fase;
    *total_ativos = qtd_obstaculos + qtd_inimigos;

    float multiplicador_vel = 1.0 + (fase - 1) * 0.20; // +20% velocidade por fase

    // Obstáculos estáticos (rochas e naufrágios)
    for (int i = 0; i < qtd_obstaculos; i++) {
        init_enemy(&entidades[i], 0);
    }

    // Inimigos em ondas (monstros e sereias)
    for (int i = qtd_obstaculos; i < *total_ativos; i++) {
        init_enemy(&entidades[i], 1);
        entidades[i].speed *= multiplicador_vel;
        entidades[i].x = 800.0 + ((i - qtd_obstaculos) * 180.0);
    }

    al_start_timer(fase_timer);
}

// Reseta a campanha inteira (volta para a Fase 1 e reembaralha o quiz)
void iniciar_novo_jogo(Player* player, Enemy entidades[], int* fase_atual, int* total_ativos, int* tempo_restante, float* velocidade_mar, ALLEGRO_TIMER* fase_timer) {
    *fase_atual = 1;
    embaralhar_quiz();
    preparar_fase(player, entidades, *fase_atual, total_ativos, tempo_restante, velocidade_mar, fase_timer);
}

int main() {
    if (!al_init()) return -1;
    if (!al_install_keyboard()) return -1;
    if (!al_install_mouse()) return -1;
    if (!al_init_font_addon()) return -1;
    if (!al_init_ttf_addon()) return -1;
    if (!al_init_primitives_addon()) return -1;
    if (!al_init_image_addon()) return -1;

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    carregar_sprites_inimigos();

    srand((unsigned int)time(NULL));

    const int LARGURA_TELA = 800;
    const int ALTURA_TELA = 600;

    al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST); //Desativa o v-sync forçado que trava o loop

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
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(fase_timer));

    al_start_timer(timer);

    // 0 = Menu, 1 = Jogando, 2 = Vitória Final, 3 = Game Over, 4 = Quiz, 5 = Fase Concluída
    int game_state = 0;
    int fase_atual = 1;
    int total_ativos = 0;
    int quiz_atual = 0;
    int inimigo_colidido = -1;
    int up = 0, down = 0, left = 0, right = 0;
    int tempo_restante = 15;

    Player player = { 0 };
    Enemy entidades[MAX_ENTIDADES];
    ALLEGRO_BITMAP* bg_mar = al_load_bitmap("sprites/mar.png");

    // Carregamento de imagens do menu
    ALLEGRO_BITMAP* img_fundo = al_load_bitmap("sprites/fundo.png");
    ALLEGRO_BITMAP* img_titulo = al_load_bitmap("sprites/titulo.png");
    ALLEGRO_BITMAP* img_bt_jogar = al_load_bitmap("sprites/jogar.png");
    ALLEGRO_BITMAP* img_bt_config = al_load_bitmap("sprites/config.png");
    ALLEGRO_BITMAP* img_bt_creditos = al_load_bitmap("sprites/creditos.png");
    ALLEGRO_BITMAP* img_bt_sair = al_load_bitmap("sprites/sair.png");

    int bt_width = 200;
    int bt_height = 40;
    int bt_x = (LARGURA_TELA - bt_width) / 2;
    int bt_jogar_y = 260;
    int bt_config_y = 320;
    int bt_creditos_y = 380;
    int bt_sair_y = 440;

    float scroll_x = 0.0;
    float velocidade_mar = 2.5;

    bool rodando = true;
    bool redesenhar = false;

    while (rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }

        // Clique do mouse na Tela Inicial (Menu)
        if (game_state == 0 && event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            int mouse_x = event.mouse.x;
            int mouse_y = event.mouse.y;

            if (mouse_x >= bt_x && mouse_x <= bt_x + bt_width &&
                mouse_y >= bt_jogar_y && mouse_y <= bt_jogar_y + bt_height) {
                al_flush_event_queue(event_queue);
                iniciar_novo_jogo(&player, entidades, &fase_atual, &total_ativos, &tempo_restante, &velocidade_mar, fase_timer);
                game_state = 1;
            }
            else if (mouse_x >= bt_x && mouse_x <= bt_x + bt_width &&
                mouse_y >= bt_sair_y && mouse_y <= bt_sair_y + bt_height) {
                rodando = false;
            }
        }

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (game_state == 0) {
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE || event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    iniciar_novo_jogo(&player, entidades, &fase_atual, &total_ativos, &tempo_restante, &velocidade_mar, fase_timer);
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
                    iniciar_novo_jogo(&player, entidades, &fase_atual, &total_ativos, &tempo_restante, &velocidade_mar, fase_timer);
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
            else if (game_state == 5) {
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE || event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    fase_atual++;
                    preparar_fase(&player, entidades, fase_atual, &total_ativos, &tempo_restante, &velocidade_mar, fase_timer);
                    game_state = 1;
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

        // Contagem regressiva da fase
        if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == fase_timer) {
            if (game_state == 1) {
                tempo_restante--;
                if (tempo_restante <= 0) {
                    al_stop_timer(fase_timer);
                    up = down = left = right = 0;
                    if (fase_atual < TOTAL_FASES) {
                        game_state = 5;
                    }
                    else {
                        game_state = 2;
                    }
                }
            }
        }

        // Atualização da lógica a 60 FPS (sem desenhar ainda!)
        if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == timer) {
            if (game_state == 1) {
                scroll_x -= velocidade_mar;
                if (scroll_x <= -LARGURA_TELA) {
                    scroll_x = 0.0;
                }

                update_player(&player, up, down, left, right);

                for (int i = 0; i < total_ativos; i++) {
                    update_enemy(&entidades[i]);

                    if (check_collision(player, entidades[i])) {
                        al_stop_timer(fase_timer);
                        inimigo_colidido = i;
                        if (indice_fila_quiz >= TOTAL_QUIZ) {
                            embaralhar_quiz();
                        }
                        quiz_atual = ordem_quiz[indice_fila_quiz];
                        indice_fila_quiz++;
                        up = down = left = right = 0;
                        game_state = 4;
                        break;
                    }
                }
            }
            redesenhar = true; // Marca que a tela precisa ser atualizada
        }

        // Só desenha quando TODOS os eventos de teclado e timer pendentes já foram lidos
        if (redesenhar && al_is_event_queue_empty(event_queue)) {
            redesenhar = false;

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
                if (img_fundo) {
                    al_draw_scaled_bitmap(img_fundo, 0, 0, al_get_bitmap_width(img_fundo), al_get_bitmap_height(img_fundo), 0, 0, LARGURA_TELA, ALTURA_TELA, 0);
                }
                else {
                    al_clear_to_color(al_map_rgb(20, 25, 35));
                }

                if (img_titulo) {
                    float t_w = al_get_bitmap_width(img_titulo);
                    float t_h = al_get_bitmap_height(img_titulo);
                    al_draw_scaled_bitmap(img_titulo, 0, 0, t_w, t_h, (LARGURA_TELA - 400) / 2, 60, 400, t_h * (400 / t_w), 0);
                }

                if (img_bt_jogar) al_draw_scaled_bitmap(img_bt_jogar, 0, 0, al_get_bitmap_width(img_bt_jogar), al_get_bitmap_height(img_bt_jogar), bt_x, bt_jogar_y, bt_width, bt_height, 0);
                if (img_bt_config) al_draw_scaled_bitmap(img_bt_config, 0, 0, al_get_bitmap_width(img_bt_config), al_get_bitmap_height(img_bt_config), bt_x, bt_config_y, bt_width, bt_height, 0);
                if (img_bt_creditos) al_draw_scaled_bitmap(img_bt_creditos, 0, 0, al_get_bitmap_width(img_bt_creditos), al_get_bitmap_height(img_bt_creditos), bt_x, bt_creditos_y, bt_width, bt_height, 0);
                if (img_bt_sair) al_draw_scaled_bitmap(img_bt_sair, 0, 0, al_get_bitmap_width(img_bt_sair), al_get_bitmap_height(img_bt_sair), bt_x, bt_sair_y, bt_width, bt_height, 0);
            }
            else if (game_state == 1) {
                draw_player(player);
                for (int i = 0; i < total_ativos; i++) {
                    draw_enemy(entidades[i]);
                }

                al_draw_textf(font, al_map_rgb(255, 255, 255), 20, 20, 0, "Tempo: %ds", tempo_restante);
                al_draw_textf(font, al_map_rgb(255, 215, 0), LARGURA_TELA - 20, 20, ALLEGRO_ALIGN_RIGHT, "%s (%d/%d)", nomes_fases[fase_atual - 1], fase_atual, TOTAL_FASES);
            }
            else if (game_state == 2) {
                al_draw_filled_rectangle(100, 190, 700, 390, al_map_rgba(10, 15, 30, 220));
                al_draw_rectangle(100, 190, 700, 390, al_map_rgb(100, 255, 100), 3.0);

                al_draw_text(font, al_map_rgb(100, 255, 100), LARGURA_TELA / 2, 230, ALLEGRO_ALIGN_CENTER, "GLORIA A ODISSEU! VOCE CHEGOU A ITACA!");
                al_draw_text(font, al_map_rgb(220, 220, 220), LARGURA_TELA / 2, 275, ALLEGRO_ALIGN_CENTER, "Voce superou todos os 5 mares e desafios dos deuses.");
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 330, ALLEGRO_ALIGN_CENTER, "Pressione 'R' para Jogar Novamente ou 'M' para o Menu");
            }
            else if (game_state == 3) {
                al_draw_filled_rectangle(120, 190, 680, 390, al_map_rgba(10, 15, 30, 220));
                al_draw_rectangle(120, 190, 680, 390, al_map_rgb(255, 60, 60), 3.0);

                al_draw_text(font, al_map_rgb(255, 60, 60), LARGURA_TELA / 2, 230, ALLEGRO_ALIGN_CENTER, "GAME OVER - O navio naufragou!");
                al_draw_textf(font, al_map_rgb(220, 220, 220), LARGURA_TELA / 2, 275, ALLEGRO_ALIGN_CENTER, "Voce caiu na %s", nomes_fases[fase_atual - 1]);
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 330, ALLEGRO_ALIGN_CENTER, "Pressione 'R' para Tentar Novamente ou 'M' para o Menu");
            }
            else if (game_state == 4) {
                draw_player(player);
                for (int i = 0; i < total_ativos; i++) {
                    draw_enemy(entidades[i]);
                }
                al_draw_textf(font, al_map_rgb(255, 255, 255), 20, 20, 0, "Tempo: %ds", tempo_restante);
                al_draw_textf(font, al_map_rgb(255, 215, 0), LARGURA_TELA - 20, 20, ALLEGRO_ALIGN_RIGHT, "%s (%d/%d)", nomes_fases[fase_atual - 1], fase_atual, TOTAL_FASES);

                al_draw_filled_rectangle(60, 190, 740, 410, al_map_rgba(10, 15, 30, 235));
                al_draw_rectangle(60, 190, 740, 410, al_map_rgb(255, 215, 0), 3.0);

                al_draw_text(font, al_map_rgb(255, 215, 0), LARGURA_TELA / 2, 220, ALLEGRO_ALIGN_CENTER, "DESAFIO DOS DEUSES! RESPONDA PARA CONTINUAR:");
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 275, ALLEGRO_ALIGN_CENTER, banco_quiz[quiz_atual].pergunta);
                al_draw_text(font, al_map_rgb(100, 255, 150), LARGURA_TELA / 2, 330, ALLEGRO_ALIGN_CENTER, banco_quiz[quiz_atual].opcoes);
                al_draw_text(font, al_map_rgb(180, 180, 180), LARGURA_TELA / 2, 380, ALLEGRO_ALIGN_CENTER, "Pressione 1, 2 ou 3 no teclado");
            }
            else if (game_state == 5) {
                draw_player(player);

                al_draw_filled_rectangle(100, 185, 700, 405, al_map_rgba(10, 15, 30, 230));
                al_draw_rectangle(100, 185, 700, 405, al_map_rgb(100, 200, 255), 3.0);

                al_draw_textf(font, al_map_rgb(100, 255, 150), LARGURA_TELA / 2, 220, ALLEGRO_ALIGN_CENTER, "%s CONCLUIDA!", nomes_fases[fase_atual - 1]);
                al_draw_textf(font, al_map_rgb(255, 215, 0), LARGURA_TELA / 2, 265, ALLEGRO_ALIGN_CENTER, "Proximo destino: %s", nomes_fases[fase_atual]);
                al_draw_textf(font, al_map_rgb(200, 200, 200), LARGURA_TELA / 2, 305, ALLEGRO_ALIGN_CENTER, "Novo tempo de travessia: %d segundos", 10 + ((fase_atual + 1) * 5));
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, 360, ALLEGRO_ALIGN_CENTER, "Pressione ESPACO ou ENTER para Continuar");
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
    destruir_sprites_inimigos();
    if (img_fundo) al_destroy_bitmap(img_fundo);
    if (img_titulo) al_destroy_bitmap(img_titulo);
    if (img_bt_jogar) al_destroy_bitmap(img_bt_jogar);
    if (img_bt_config) al_destroy_bitmap(img_bt_config);
    if (img_bt_creditos) al_destroy_bitmap(img_bt_creditos);
    if (img_bt_sair) al_destroy_bitmap(img_bt_sair);
    if (bg_mar) al_destroy_bitmap(bg_mar);

    return 0;
}