#ifndef SCREENSTER_H
#define SCREENSTER_H

#include <assert.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "buf.h"
#include "mpool.h"
#include "types.h"

//
// Built in object type constants

extern int kObjectTypeScreen;
extern int kObjectTypeImage;
extern int kObjectTypeTileset;

//
// Defaults

#define DEFAULT_PORT            32000

//
// Custom event types

enum {
    SCREENSTER_EVENT_DISCONNECT         = 512,
    SCREENSTER_EVENT_MESSAGE_RECEIVED
};

//
// Maximum incoming message size

#define MAX_MSG_ARGS_SIZE       4092
#define MAX_MSG_SIZE            4096

//
//

#define HANDLER_ARGS conn_t *conn, msg_t *msg
#define HANDLER_FN(name) void name(HANDLER_ARGS)
typedef void (*handler_f)(HANDLER_ARGS);

//
// Memory pool for use by command handlers
// Any allocated memory will be released after the handler returns

#define HANDLER_ALLOC(sz)       mpool_alloc(&conn->arg_pool, sz)
#define HANDLER_POOL_SIZE       65536

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
    
// Convert a color (expressed as uint32_t ARGB) to an ALLEGRO_COLOR
#define COLOR_TO_ALLEGRO(argb) \
    al_map_rgba( \
        ((argb) >> 16) & 0xFF, \
        ((argb) >>  8) & 0xFF, \
        ((argb) >>  0) & 0xFF, \
        ((argb) >> 24) & 0xFF \
    )

//
// Global state (see `state.c`)

extern obj_id_t         active_screen_id;
extern draw_state_t     draw_state;

//
//

void        obj_init(void);
int         obj_register_type(obj_destructor_f destructor);
void*       obj_create(int type, ssize_t sz);
void        obj_retain(void *obj);
void        obj_release(void *obj);
void        obj_release_id(obj_id_t id);
obj_t*      obj_get(obj_id_t id);
obj_t*      obj_get_safe(obj_id_t id, int type);
void        obj_destroy_all(void);

void        register_builtin_types(void);

void        create_category(unsigned char category, int function_count);
void        install_handler(unsigned char category, unsigned char offset, handler_f fn);
handler_f   get_handler(unsigned char category, unsigned char command);

status_t    msg_pool_init(msg_pool_t *pool);
void        msg_pool_cleanup(msg_pool_t *pool);
msg_t*      msg_pool_current(msg_pool_t *pool);
msg_t*      msg_pool_checkout(msg_pool_t *pool);
void        msg_pool_return(msg_pool_t *pool, msg_t *msg);

obj_id_t    create_screen(int width, int height, int al_flags);
status_t    activate_screen(obj_id_t screen_id);

obj_id_t    create_image(const char *filename);

obj_id_t    load_tileset_from_file(const char *filename, uint16_t tile_width, uint16_t tile_height, uint32_t mask_color);
obj_id_t    create_tileset_from_image(obj_id_t image_id, uint16_t tile_width, uint16_t tile_height, uint32_t mask_color);

void        reset_graphics_state();
void        save_graphics_state();
void        restore_graphics_state();

#endif