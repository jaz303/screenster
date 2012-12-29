#include "mpool.h"

#define MPOOL_ALIGN 8
#define MPOOL_ALIGN_SZ(sz) \
    (((sz) + (MPOOL_ALIGN - 1)) & ~(MPOOL_ALIGN - 1))

mpool_t* mpool_create(ssize_t sz) {
    mpool_t *pool = malloc(sizeof(mpool_t));
    if (pool) {
        pool->size = MPOOL_ALIGN_SZ(sz);
        pool->base = malloc(pool->size);
        if (pool->base) {
            pool->end = pool->base + pool->size;
            pool->next = pool->base;
        } else {
            free(pool);
            pool = 0;
        }
    }
    return pool;
}

void mpool_destroy(mpool_t* pool) {
    if (pool) {
        free(pool->base);
        free(pool);
    }
}

void* mpool_alloc(mpool_t* pool, ssize_t sz) {
    sz = MPOOL_ALIGN_SZ(sz);
    if (pool->next + sz > pool->end) {
        return 0;
    }
    char *ptr = pool->next;
    pool->next += sz;
    return ptr;
}

void mpool_clear(mpool_t* pool) {
    pool->next = pool->base;
}