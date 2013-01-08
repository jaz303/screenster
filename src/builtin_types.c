#include "screenster.h"

static int type_screen;
static int type_image;
static int type_tileset;

static void destroy_screen(obj_t *obj);
static void destroy_image(obj_t *obj);
static void destroy_tileset(obj_t *obj);

typedef struct {
    obj_t header;
    ALLEGRO_DISPLAY *display;
} obj_screen_t;

void register_builtin_types(void) {
    type_screen = obj_register_type(destroy_screen);
    type_image = obj_register_type(destroy_image);
    type_tileset = obj_register_type(destroy_tileset);
}

obj_id_t create_screen(int width, int height, int al_flags) {
    obj_screen_t *screen = obj_create(type_screen, sizeof(obj_screen_t));
    if (!screen)
        return 0;
        
    al_set_new_display_flags(al_flags);
    screen->display = al_create_display(width, height);
    
    if (!screen->display) {
        obj_release(screen);
        return 0;
    }
    
    return screen->header.id;
}

status_t activate_screen(obj_id_t screen_id) {
    obj_screen_t *screen = OBJ_GET_SAFE(screen_id, obj_screen_t*, type_screen);
    if (!screen) {
        return GEN_ERROR;
    }
    active_screen_id = screen_id;
    al_set_target_backbuffer(screen->display);
    return OK;
}

void destroy_screen(obj_t *obj) {
    if (active_screen_id == obj->id) {
        active_screen_id = 0;
        al_set_target_bitmap(NULL);
    }
    al_destroy_display(((obj_screen_t*)obj)->display);
}

void destroy_image(obj_t *obj) {
    
}

void destroy_tileset(obj_t *obj) {
    
}
