#ifndef SCREENSTER_H
#define SCREENSTER_H

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "types.h"

//
// Built in object type constants

extern int kObjectTypeScreen;
extern int kObjectTypeImage;
extern int kObjectTypeTileset;

//
//

#define OK                      0
#define GEN_ERROR               -1

#define SUCCESS(stat)           ((stat) == OK)
#define ERROR(stat)             ((stat) != OK)

#define OBJ_SUCCESS(obj_id)     ((obj_id) > 0)
#define OBJ_ERROR(obj_id)       ((obj_id) == 0)

#define OBJ_GET_SAFE(obj_id, c_type, obj_type) \
    (c_type)obj_get_safe(obj_id, obj_type)

//
// Global state (see `state.c`)

extern obj_id_t active_screen_id;

//
//

void obj_init(void);
int obj_register_type(obj_destructor_f destructor);
void* obj_create(int type, ssize_t sz);
void obj_retain(void *obj);
void obj_release(void *obj);
obj_t* obj_get(obj_id_t id);
obj_t* obj_get_safe(obj_id_t id, int type);
void obj_destroy_all(void);

void register_builtin_types(void);

obj_id_t create_screen(int width, int height, int al_flags);
status_t activate_screen(obj_id_t screen_id);

obj_id_t create_image(const char *filename);

#endif