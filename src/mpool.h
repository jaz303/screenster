#ifndef MPOOL_H
#define MPOOL_H

#include <stdlib.h>
#include <sys/types.h>

typedef struct {
    ssize_t         size;
    char            *base;
    char            *end;
    char            *next;
} mpool_t;

int             mpool_init(mpool_t *pool, ssize_t sz);
int             mpool_cleanup(mpool_t *pool);
void*           mpool_alloc(mpool_t* pool, ssize_t sz);
void            mpool_clear(mpool_t* pool);

#endif