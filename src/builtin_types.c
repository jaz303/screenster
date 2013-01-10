#include "screenster.h"

int kObjectTypeScreen;
int kObjectTypeImage;
int kObjectTypeTileset;

static void destroy_screen(obj_t *obj);
static void destroy_image(obj_t *obj);
static void destroy_tileset(obj_t *obj);

void register_builtin_types(void) {
    kObjectTypeScreen = obj_register_type(destroy_screen);
    kObjectTypeImage = obj_register_type(destroy_image);
    kObjectTypeTileset = obj_register_type(destroy_tileset);
}

//
// Screen

obj_id_t create_screen(int width, int height, int al_flags) {
    obj_screen_t *screen = obj_create(kObjectTypeScreen, sizeof(obj_screen_t));
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
    obj_screen_t *screen = OBJ_GET_SAFE(screen_id, obj_screen_t*, kObjectTypeScreen);
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
    
    obj_screen_t *screen = (obj_screen_t*)obj;
    if (screen->display) {
        al_destroy_display(screen->display);
    }

}

//
// Image

obj_id_t create_image(const char *filename) {
    obj_image_t *image = obj_create(kObjectTypeImage, sizeof(obj_image_t));
    if (!image)
        return 0;
    
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ARGB_8888);
    image->bitmap = al_load_bitmap(filename);
    
    if (!image->bitmap) {
        obj_release(image);
        return 0;
    }
    
    return image->header.id;
}

void destroy_image(obj_t *obj) {
    obj_image_t *image = (obj_image_t*)obj;
    if (image->bitmap) {
        al_destroy_bitmap(image->bitmap);
    }
}

void destroy_tileset(obj_t *obj) {
    
}
