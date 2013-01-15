#ifndef SCREENSTER_TYPES_H
#define SCREENSTER_TYPES_H

typedef uint32_t obj_id_t;

typedef struct {
    uint8_t     type;
    uint8_t     refcount;
    obj_id_t    id;
} obj_t;

typedef struct {
    obj_t header;
    ALLEGRO_DISPLAY *display;
} obj_screen_t;

typedef struct {
    obj_t header;
    ALLEGRO_BITMAP *bitmap;
} obj_image_t;

typedef struct {
    float x;
    float y;
} tileinfo_t;

typedef struct {
    obj_t header;
    obj_image_t *image;
    ALLEGRO_BITMAP *bitmap;
    int width, height;
    int tile_width, tile_height;
    int tile_count;
    tileinfo_t *tiles;
} obj_tileset_t;

typedef void (*obj_destructor_f)(obj_t* obj);

typedef int status_t;

#endif