#ifndef SCREENSTER_TYPES_H
#define SCREENSTER_TYPES_H

#include <pthread.h>

typedef int status_t;

typedef uint32_t obj_id_t;

enum {
    PARSE_OUT,
    PARSE_GOT_HEADER
};

enum {
    PARSE_COMPLETE,
    PARSE_INCOMPLETE,
    PARSE_ARGS_TOO_LONG
};

typedef struct msg msg_t;
struct msg {
    uint8_t     cmd_category;
    uint8_t     cmd_command;
    uint16_t    cmd_len;
    int         parse_state;
    msg_t       *next_free;
    msg_t       *next;
    buf_t       args;
};

typedef struct {
    msg_t               *free;
    msg_t               *all;
    pthread_mutex_t     lock;
} msg_pool_t;

typedef struct {
    int                     fd;
    struct sockaddr_in      client_addr;
    socklen_t               client_addr_len;
    ALLEGRO_EVENT_SOURCE    event_source;
    msg_pool_t              msg_pool;
    mpool_t                 arg_pool;
} conn_t;

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

enum {
    DRAW_PEN_ON = 1,
    DRAW_FILL_ON = 2
};

typedef struct {
    unsigned int    flags;
    ALLEGRO_COLOR   pen_color;
    ALLEGRO_COLOR   fill_color;
    float           x;
    float           y;
} draw_state_t;

#endif