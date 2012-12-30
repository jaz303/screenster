#ifndef VECTOR_H
#define VECTOR_H

#include <sys/types.h>

typedef struct {
    ssize_t         capacity;
    ssize_t         length;
    void**          items;
} vector_t;

vector_t*       vector_create();
void            vector_destroy(vector_t *vec);

ssize_t         vector_length(vector_t *vec);
void*           vector_get(vector_t *vec, ssize_t ix);
ssize_t         vector_set(vector_t *vec, ssize_t ix, void* val);
ssize_t         vector_push(vector_t *vec, void *val);
void*           vector_pop(vector_t *vec);


#endif