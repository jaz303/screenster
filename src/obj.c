#include "screenster.h"

#define MAX_OBJ_TYPES 255
#define INITIAL_OBJS 256

static int obj_next_type = 1;
static obj_destructor_f obj_type_destructors[MAX_OBJ_TYPES + 1] = {0};

static obj_id_t obj_all_sz;
static obj_t    **obj_all;
static obj_id_t *obj_free;
static obj_id_t obj_free_ix;

static obj_id_t next_obj_id() {
    if (obj_free_ix == obj_all_sz) {
        
        obj_all_sz *= 2;
        obj_all = realloc(obj_all, sizeof(obj_t*) * obj_all_sz);
        obj_free = realloc(obj_free, sizeof(obj_id_t) * obj_all_sz);
        
        for (int i = obj_free_ix; i < obj_all_sz; i++) {
            obj_all[i] = NULL;
            obj_free[i] = i;
        }
    
    }
    return obj_free[obj_free_ix++];
}

static void return_obj_id(obj_id_t id) {
    obj_free[--obj_free_ix] = id;
}

void obj_init(void) {
    
    obj_all_sz = INITIAL_OBJS;
    obj_all = malloc(sizeof(obj_t*) * obj_all_sz);
    obj_free = malloc(sizeof(obj_id_t) * obj_all_sz);
    obj_free_ix = 0;
    
    for (int i = 0; i < obj_all_sz; i++) {
        obj_all[i] = NULL;
        obj_free[i] = i;
    }
    
    next_obj_id(); // consume id 0 and never return it (zero indicates error).

}

int obj_register_type(obj_destructor_f destructor) {
    
    if (obj_next_type > MAX_OBJ_TYPES)
        return -1;
        
    int new_obj_type = obj_next_type++;
    
    obj_type_destructors[new_obj_type] = destructor;
    
    return new_obj_type;

}

void* obj_create(int type, ssize_t sz) {
    
    obj_id_t id = next_obj_id();
    if (id == 0)
        return 0;
    
    obj_t* obj = malloc(sz);
    if (!obj) {
        return_obj_id(id);
        return 0;
    }
        
    obj->type = type;
    obj->refcount = 1;
    obj->id = id;
    
    obj_all[obj->id] = obj;
    
    return obj;
    
}

void obj_retain(void *obj) {
    ((obj_t*)obj)->refcount++;
}

static void obj_destroy(obj_t *obj) {
    obj_type_destructors[obj->type](obj);
    free(obj);
    obj_all[obj->id] = 0;
    return_obj_id(obj->id);
}

void obj_release(void *obj) {
    if (--((obj_t*)obj)->refcount == 0) {
        obj_destroy(obj);
    }
}

void obj_release_id(obj_id_t id) {
    obj_t *obj = obj_get(id);
    if (obj) {
        obj_release(obj);
    }
}

obj_t* obj_get(obj_id_t id) {
    return obj_all[id];
}

obj_t* obj_get_safe(obj_id_t id, int type) {
    obj_t *obj = obj_all[id];
    if (obj != NULL && type > 0 && obj->type != type) {
        obj = NULL;
    }
    return obj;
}

void obj_destroy_all(void) {
    for (int i = 0; i < obj_all_sz; i++) {
        if (obj_all[i])
            obj_destroy(obj_all[i]);
    }
}
