#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

int main() {
    //Iniciar o allegro e a fonte dele.
    al_init();
    al_init_font_addon();

    //Para renderizar a janela com suas dimensões, e abaixo, onde ela vai aparecer na tela.
    ALLEGRO_DISPLAY * display = al_create_display(640, 480);
    al_set_window_position(display, 200, 200);

    //Fonte embutida do proprio allegro e em seguida, resumidamente, um fps.
    ALLEGRO_FONT * font = al_create_builtin_font();
    ALLEGRO_TIMER * timer = al_create_timer(1.0 / 60.0);

    //Para conseguir fechar a janela clicando no x.
    ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    while (true){
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        if( event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            break;
        }
        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_draw_text(font, al_map_rgb(0, 0, 0), 230, 200, 0, "Isso realmente será uma Odisséia.");
        al_flip_display();
    }

    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
    
}