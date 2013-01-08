#ifndef SCREENSTER_TYPES_H
#define SCREENSTER_TYPES_H

typedef uint32_t obj_id_t;

typedef struct {
    uint8_t     type;
    uint8_t     refcount;
    obj_id_t    id;
} obj_t;

typedef void (*obj_destructor_f)(obj_t* obj);

typedef int status_t;

#endif