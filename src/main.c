#include <stdio.h>
#include "display.h"

bool is_running = false;

void setup(void) {
    // allocate the required memory in bytes to hold the color buffer
    color_buffer = (uint32_t *) malloc(sizeof(uint32_t) * window_width * window_height);

    // creating a sdl texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            window_width,
            window_height
    );
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
            break;
    }
}

void update(void) {
    //todo
}

void render(void) {
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    clear_color_buffer(0xFF000000);
    draw_pixel(2, 2, 0xFFFFFF00);
    draw_grid(16, 0xFF007700);
    draw_rect(window_width / 2 - window_width / 4,
              window_height / 2 - window_height / 4,
              (window_width / 2 + window_width / 4) - (window_width / 2 - window_width / 4),
              (window_height / 2 + window_height / 4) - (window_height / 2 - window_height / 4), 0xFF00FF00);

    render_color_buffer();

    SDL_RenderPresent(renderer);
}

int main(void) {
    is_running = intialize_window();

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}
