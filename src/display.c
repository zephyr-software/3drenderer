#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

int window_width = 640;
int window_height = 480;

bool intialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error intializing SDL.\n");
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
//    window_width = display_mode.w / 4 * 3;
    window_width = 512;
//    window_height = display_mode.h / 4 * 3;
    window_height = 512;

    // create a sdl window
    Uint32 flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP;
    window = SDL_CreateWindow(
            NULL,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width,
            window_height,
            flags
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    // create a sdl renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    return true;
}

void destroy_window(void) {
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void clear_color_buffer(uint32_t color) {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            color_buffer[(window_width * y) + x] = color;
        }
    }
}

void render_color_buffer(void) {
    SDL_UpdateTexture(
            color_buffer_texture,
            NULL,
            color_buffer,
            (int) (window_width * sizeof(uint32_t))
    );

    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void draw_pixel(int x, int y, uint32_t color) {
    if (x < window_width && y < window_height) {
        color_buffer[y * window_width + x] = color;
    }
}

void draw_grid(int grid_size, uint32_t color) {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            if (y % grid_size == 0 || x % grid_size == 0) {
                color_buffer[(window_width * y) + x] = color;
            }
        }
    }
}

void draw_center(uint32_t color) {
    for (int x = 0; x < window_width; x++) {
        color_buffer[window_width * (window_height / 2) + x] = color;
    }

    for (int y = 0; y < window_height; y++) {
        color_buffer[window_width * y + window_width / 2] = color;
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
    if (x + width <= 0 || y + height <= 0 || x >= window_width || y >= window_height) {
        return;
    }

    if (x < 0) {
        width += x;
        x = 0;
    }

    if (y < 0) {
        height += y;
        y = 0;
    }

    if (x + width > window_width) {
        width = window_width - x;
    }

    if (y + height > window_height) {
        height = window_height - y;
    }

    for (int j = y; j < y + height; j++) {
        for (int i = x; i < x + width; i++) {
            color_buffer[(window_width * j) + i] = color;
        }
    }
}
