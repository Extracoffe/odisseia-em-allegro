//Código para testar

#include <allegro5/allegro.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if (!al_init()) {
        printf("Falha ao inicializar o Allegro!\n");
        return -1;
    }

    ALLEGRO_DISPLAY *display = al_create_display(640, 480);
    if (!display) {
        printf("Falha ao criar a janela!\n");
        return -1;
    }

    // Pinta a tela de vermelho (R=255, G=0, B=0)
    al_clear_to_color(al_map_rgb(0, 255, 0));
    al_flip_display(); // Atualiza o monitor com o que foi desenhado

    al_rest(3.0); // Espera 3 segundos

    al_destroy_display(display);
    return 0;
}