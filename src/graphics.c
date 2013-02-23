#include "screenster.h"

typedef struct {
    draw_state_t        draw_state;
} saved_graphics_state_t;

#define GRAPHICS_STACK_SIZE 32
static saved_graphics_state_t  graphics_stack[GRAPHICS_STACK_SIZE];
static int                     graphics_stack_count = 0;

void reset_graphics_state() {
    graphics_stack_count = 0;
    
    draw_state.flags = DRAW_PEN_ON | DRAW_FILL_ON;
    draw_state.pen_color = al_map_rgb(255, 255, 255);
    draw_state.fill_color = al_map_rgb(0, 0, 0);
    draw_state.x = 0.0f;
    draw_state.y = 0.0f;
}

void save_graphics_state() {
    graphics_stack[graphics_stack_count].draw_state = draw_state;
    graphics_stack_count++;
}

void restore_graphics_state() {
    graphics_stack_count--;
    draw_state = graphics_stack[graphics_stack_count].draw_state;
}