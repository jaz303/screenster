#include "rbuf.h"

#define RBUF_REMAIN(n) \
    ((buf->capacity - buf->count) >= n)

#define RBUF_PUSH(b) \
    *(buf->head++) = (b); \
    if (buf->head == buf->end) \
        buf->head = buf->data; \
    ++(buf->count)

#define RBUF_SHIFT(o) \
    o = *(buf->tail++); \
    if (buf->tail == buf->end) \
        buf->tail = buf->data; \
    --(buf->count)

rbuf_t* rbuf_create(size_t capacity) {
    rbuf_t *buf = malloc(sizeof(rbuf_t) + capacity);
    if (buf) {
        buf->capacity = capacity;
        buf->count = 0;
        buf->head = buf->tail = buf->data;
        buf->end = buf->data + buf->capacity;
    }
    return buf;
}

void rbuf_destroy(rbuf_t *buf) {
    if (buf) free(buf);
}

void rbuf_clear(rbuf_t *buf) {
    buf->count = 0;
    buf->head = buf->tail = buf->data;
}

size_t rbuf_count(rbuf_t *buf) {
    return buf->count;
}

size_t rbuf_remain(rbuf_t *buf) {
    return buf->capacity - buf->count;
}

int rbuf_is_empty(rbuf_t *buf) {
    return buf->count == 0;
}

int rbuf_is_full(rbuf_t *buf) {
    return buf->count == buf->capacity;
}

int rbuf_write(rbuf_t *buf, unsigned char *bytes, size_t len) {
    if (!RBUF_REMAIN(len)) return 0;
    while (len--) {
        RBUF_PUSH(*(bytes++));
    }
    return 1;
}

int rbuf_write_uint64(rbuf_t *buf, uint64_t v) {
    return rbuf_write(buf, (unsigned char*)&v, 8);
}

int rbuf_write_uint32(rbuf_t *buf, uint32_t v) {
    return rbuf_write(buf, (unsigned char*)&v, 4);
}

int rbuf_write_uint16(rbuf_t *buf, uint16_t v) {
    return rbuf_write(buf, (unsigned char*)&v, 2);
}

int rbuf_write_byte(rbuf_t *buf, unsigned char v) {
    if (!RBUF_REMAIN(1)) return 0;
    RBUF_PUSH(v);
    return 1;
}

int rbuf_read(rbuf_t *buf, unsigned char *out, size_t len) {
    if (buf->count < len) return 0;
    while (len--) {
        RBUF_SHIFT(*(out++));
    }
    return len;
}

#define PRIMITIVE_READER(type) \
    type##_t rbuf_read_##type(rbuf_t *buf) { \
        type##_t v; \
        rbuf_read(buf, (unsigned char*)&v, sizeof(type##_t)); \
        return v; \
    }
    
PRIMITIVE_READER(uint64)
PRIMITIVE_READER(uint32)
PRIMITIVE_READER(uint16)
PRIMITIVE_READER(int64)
PRIMITIVE_READER(int32)
PRIMITIVE_READER(int16)

unsigned char rbuf_read_byte(rbuf_t *buf) {
    if (buf->count == 0) return 0;
    unsigned char v;
    RBUF_SHIFT(v);
    return v;
}