#include <stdio.h>
#include "display.h"
#include "vector.h"

#define N_POINTS (9 * 9 * 9)

bool is_running = false;

vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];
float fov_factor = 512;
vec3_t camera_position = {.x = 0, .y = 0, .z = -4};

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

    int point_count = 0;
    for (float x = -1; x <= 1; x += 0.25) {
        for (float y = -1; y <= 1; y += 0.25) {
            for (float z = -1; z <= 1; z += 0.25) {
                vec3_t new_point = {.x = x, .y = y, .z = z};
                cube_points[point_count++] = new_point;
            }
        }
    }
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

vec2_t project(vec3_t point) {
    vec2_t projected_point = {
            .x = (fov_factor * point.x) / point.z,
            .y = (fov_factor * point.y) / point.z,
    };

    return projected_point;
}

void update(void) {
    for (int i = 0; i < N_POINTS; i++) {
        vec3_t point = cube_points[i];

        point.z -= camera_position.z;

        vec2_t projected_point = project(point);
        projected_points[i] = projected_point;
    }
}

void render(void) {
    clear_color_buffer(0xFF000000);
    draw_grid(16, 0xFF003300);
    draw_center(0xFF770000);

    uint32_t color = 0xFFFF00FF;
    for (int i = 0; i < N_POINTS; i++) {
        vec2_t projected_point = projected_points[i];
        draw_rect(projected_point.x + window_width / 2,
                  projected_point.y + window_height / 2,
                  4, 4, color);

//        color = color - 0x00100010;
//        if (color < 0xFF7F007F) {
//            color = 0xFFFF00FF;
//        }
    }


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
