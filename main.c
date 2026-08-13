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

    al_clear_to_color(al_map_rgb(0, 255, 0));
    al_flip_display(); 

    al_rest(3.0); 

    al_destroy_display(display);
    return 0;
}