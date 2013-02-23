#include "buf.h"

#define BUF_WRITE_REMAIN(n)     ((buf->capacity - buf->write) >= n)
#define BUF_READ_REMAIN(n)      ((buf->write - buf->read) >= n)

buf_t* buf_create(ssize_t capacity) {
    buf_t *buf = malloc(sizeof(buf_t) + capacity);
    if (buf) {
        buf->capacity = capacity;
        buf->read = 0;
        buf->write = 0;
    }
    return buf;
}

void buf_destroy(buf_t *buf) {
    if (buf) free(buf);
}

void buf_reset(buf_t *buf) {
    buf->read = buf->write = 0;
}

ssize_t buf_size(buf_t *buf) {
    return buf->write;
}

ssize_t buf_read_pos(buf_t *buf) {
    return buf->read;
}

ssize_t buf_write_remain(buf_t *buf) {
    return buf->capacity - buf->write;
}

ssize_t buf_read_remain(buf_t *buf) {
    return buf->write - buf->read;
}

unsigned char* buf_write_ptr(buf_t *buf) {
    return buf->data + buf->write;
}

unsigned char* bug_read_ptr(buf_t *buf) {
    return buf->data + buf->read;
}

int buf_is_empty(buf_t *buf) {
    return buf->write == 0;
}

int buf_is_full(buf_t *buf) {
    return buf->write == buf->capacity;
}

void buf_advance(buf_t *buf, ssize_t len) {
    buf->write += len;
}

void buf_truncate(buf_t *buf, ssize_t len) {
    buf->write = len;
    if (buf->read > buf->write) {
        buf->read = buf->write;
    }
}

int buf_write(buf_t *buf, unsigned char *bytes, ssize_t len) {
    if (!BUF_WRITE_REMAIN(len)) return 0;
    memcpy(buf->data + buf->write, bytes, len);
    buf->write += len;
    return 1;
}

#define PRIMITIVE_WRITER(type, c_type) \
    int buf_write_##type(buf_t *buf, c_type v) { \
        return buf_write(buf, (unsigned char*)&v, sizeof(c_type)); \
    }

PRIMITIVE_WRITER(uint64, uint64_t)
PRIMITIVE_WRITER(uint32, uint32_t)
PRIMITIVE_WRITER(uint16, uint16_t)
PRIMITIVE_WRITER(int64, int64_t)
PRIMITIVE_WRITER(int32, int32_t)
PRIMITIVE_WRITER(int16, int16_t)
PRIMITIVE_WRITER(float, float)
PRIMITIVE_WRITER(double, double)

int buf_write_byte(buf_t *buf, unsigned char v) {
    if (!BUF_WRITE_REMAIN(1)) return 0;
    buf->data[buf->write++] = v;
    return 1;
}

int buf_read(buf_t *buf, unsigned char *out, ssize_t len) {
    if (!BUF_READ_REMAIN(len)) return 0;
    memcpy(out, buf->data + buf->read, len);
    buf->read += len;
    return len;
}

#define PRIMITIVE_READER(type, c_type) \
    c_type buf_read_##type(buf_t *buf) { \
        c_type v; \
        buf_read(buf, (unsigned char*)&v, sizeof(c_type)); \
        return v; \
    }
    
PRIMITIVE_READER(uint64, uint64_t)
PRIMITIVE_READER(uint32, uint32_t)
PRIMITIVE_READER(uint16, uint16_t)
PRIMITIVE_READER(int64, int64_t)
PRIMITIVE_READER(int32, int32_t)
PRIMITIVE_READER(int16, int16_t)
PRIMITIVE_READER(float, float)
PRIMITIVE_READER(double, double)

unsigned char buf_read_byte(buf_t *buf) {
    if (!BUF_READ_REMAIN(1)) return 0;
    return buf->data[buf->read++];
}