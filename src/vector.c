#include "vector.h"

#include <stdlib.h>

// must be at least 2
#define VECTOR_INITIAL_CAPACITY 8

static int grow(vector_t *vec, ssize_t min_sz) {
    if (vec->capacity < min_sz) {
        ssize_t new_capacity = vec->capacity;
        while (new_capacity < min_sz) {
            new_capacity *= 2;
        }
        void** new_items = realloc(vec->items, new_capacity);
        if (!new_items) {
            return 0;
        }
        vec->capacity = new_capacity;
        vec->items = new_items;
    }
    return 1;
}

vector_t* vector_create() {
    vector_t *vec = malloc(sizeof(vector_t));
    if (vec) {
        vec->capacity = VECTOR_INITIAL_CAPACITY / 2;
        vec->length = 0;
        vec->items = NULL;
        if (!grow(vec, VECTOR_INITIAL_CAPACITY)) {
            free(vec);
            vec = 0;
        }
    }
    return vec;
}

void vector_destroy(vector_t *vec) {
    if (vec) {
        free(vec->items);
        free(vec);
    }
}

ssize_t vector_length(vector_t *vec) {
    return vec->length;
}

void* vector_get(vector_t *vec, ssize_t ix) {
    if (ix < 0 || ix >= vec->length) {
        return 0;
    } else {
        return vec->items[ix];
    }
}

ssize_t vector_set(vector_t *vec, ssize_t ix, void* val) {
    if (ix < 0 || ix >= vec->length) {
        return -1;
    } else {
        vec->items[ix] = val;
        return ix;
    }
}

ssize_t vector_push(vector_t *vec, void *val) {
    if (vec->length == vec->capacity) {
        if (!grow(vec, vec->length + 1)) {
            return -1;
        }
    }
    ssize_t ix = vec->length++;
    vec->items[ix] = val;
    return ix;
}

void* vector_pop(vector_t *vec) {
    if (vec->length == 0) {
        return 0;
    } else {
        vec->length--;
        return vec->items[vec->length];
    }
}