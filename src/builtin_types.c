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

//
// Tileset

obj_id_t load_tileset_from_file(const char *filename, uint16_t tile_width, uint16_t tile_height, uint32_t mask_color) {
    obj_id_t image = create_image(filename);
    if (OBJ_SUCCESS(image)) {
        obj_id_t tileset = create_tileset_from_image(image, tile_width, tile_height, mask_color);
        obj_release_id(image);
        return tileset;
    } else {
        return 0;        
    }
}

obj_id_t create_tileset_from_image(obj_id_t image_id, uint16_t tile_width, uint16_t tile_height, uint32_t mask_color) {
    obj_image_t *image = OBJ_GET_SAFE(image_id, obj_image_t*, kObjectTypeImage);
    if (!image)
        return 0;
    
    obj_tileset_t *tileset = obj_create(kObjectTypeTileset, sizeof(obj_tileset_t));
    if (!tileset)
        return 0;
        
    tileset->image = image;
    tileset->bitmap = image->bitmap;
    tileset->tiles = NULL;
    obj_retain(image);
        
    tileset->width = al_get_bitmap_width(image->bitmap);
    tileset->height = al_get_bitmap_height(image->bitmap);
    tileset->tile_width = tile_width;
    tileset->tile_height = tile_height;
    
    int tiles_wide = tileset->width / tileset->tile_width;
    int tiles_high = tileset->height / tileset->tile_height;
    
    tileset->tile_count = tiles_wide * tiles_high;
    
    tileset->tiles = malloc(sizeof(tileinfo_t) * tileset->tile_count);
    if (!tileset->tiles) {
        obj_release(tileset);
        return 0;
    }
    
    float tx = 0.0, ty = 0.0;
    for (int i = 0; i < tiles_high; i++) {
        tx = 0.0;
        for (int j = 0; j < tiles_wide; j++) {
            tileinfo_t *tile = &tileset->tiles[(i * tiles_wide) + j];
            tile->x = tx;
            tile->y = ty;
            tx += tileset->tile_width;
        }
        ty += tileset->tile_width;
    }
    
    if (mask_color != 0) {
        al_convert_mask_to_alpha(image->bitmap, COLOR_TO_ALLEGRO(mask_color));
    }
    
    return tileset->header.id;
}

void destroy_tileset(obj_t *obj) {
    obj_tileset_t *tileset = (obj_tileset_t*)obj;
    if (tileset->image) {
        obj_release(tileset->image);
    }
    if (tileset->tiles) {
        free(tileset->tiles);
    }
}
