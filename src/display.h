#ifndef DISPLAY_H
#define DISPLAY_H


#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>


#define FPS 25
#define FRAME_TARGET_TIME (1000 / FPS)


enum cull_method {
    CULL_NONE,
    CULL_BACKFACE
} cull_method;

enum render_method {
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE,
    RENDER_TEXTURED,
    RENDER_TEXTURED_WIRE
} render_method;


extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern uint32_t *color_buffer;
extern float *z_buffer;
extern SDL_Texture *color_buffer_texture;

extern int window_width;
extern int window_height;


bool initialize_window(void);

void destroy_window(void);

void clear_color_buffer(uint32_t color);

void clear_z_buffer(void);

void render_color_buffer(void);

void draw_pixel(int x, int y, uint32_t color);

void draw_grid(int grid_size, uint32_t color);

void draw_center(uint32_t color);

void draw_rect(int x, int y, int width, int height, uint32_t color);

void draw_line(int x0, int y0, int x1, int y1, uint32_t color);


#endif
