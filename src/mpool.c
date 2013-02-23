#include "mpool.h"

#define MPOOL_ALIGN 8
#define MPOOL_ALIGN_SZ(sz) \
    (((sz) + (MPOOL_ALIGN - 1)) & ~(MPOOL_ALIGN - 1))

int mpool_init(mpool_t *pool, ssize_t sz) {
    pool->size = MPOOL_ALIGN_SZ(sz);
    pool->base = malloc(pool->size);
    if (pool->base) {
        pool->end = pool->base + pool->size;
        pool->next = pool->base;
        return 1;
    } else {
        return 0;
    }
}

int mpool_cleanup(mpool_t *pool) {
    free(pool->base);
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