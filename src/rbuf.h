#ifndef RBUF_H
#define RBUF_H

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    size_t          capacity;
    size_t          count;
    unsigned char   *head;
    unsigned char   *tail;
    unsigned char   *end;
    unsigned char   data[0];
} rbuf_t;

rbuf_t*             rbuf_create(size_t capacity);
void                rbuf_destroy(rbuf_t *buf);

void                rbuf_clear(rbuf_t *buf);
size_t              rbuf_count(rbuf_t *buf);
size_t              rbuf_remain(rbuf_t *buf);
int                 rbuf_is_empty(rbuf_t *buf);
int                 rbuf_is_full(rbuf_t *buf);

int                 rbuf_write(rbuf_t *buf, unsigned char *bytes, size_t len);
int                 rbuf_write_uint64(rbuf_t *buf, uint64_t v);
int                 rbuf_write_uint32(rbuf_t *buf, uint32_t v);
int                 rbuf_write_uint16(rbuf_t *buf, uint16_t v);
int                 rbuf_write_int64(rbuf_t *buf, int64_t v);
int                 rbuf_write_int32(rbuf_t *buf, int32_t v);
int                 rbuf_write_int16(rbuf_t *buf, int16_t v);
int                 rbuf_write_byte(rbuf_t *buf, unsigned char v);

int                 rbuf_read(rbuf_t *buf, unsigned char *out, size_t len);
uint64_t            rbuf_read_uint64(rbuf_t *buf);
uint32_t            rbuf_read_uint32(rbuf_t *buf);
uint16_t            rbuf_read_uint16(rbuf_t *buf);
int64_t             rbuf_read_int64(rbuf_t *buf);
int32_t             rbuf_read_int32(rbuf_t *buf);
int16_t             rbuf_read_int16(rbuf_t *buf);
unsigned char       rbuf_read_byte(rbuf_t *buf);

#endif