#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
    ssize_t         capacity;
    ssize_t         read;
    ssize_t         write;
    unsigned char   data[0];
} buf_t;

buf_t*              buf_create(ssize_t capacity);
void                buf_destroy(buf_t *buf);

void                buf_init(buf_t *buf, ssize_t capacity);
                    
void                buf_reset(buf_t *buf);
ssize_t             buf_size(buf_t *buf);
ssize_t             buf_read_pos(buf_t *buf);
ssize_t             buf_write_remain(buf_t *buf);
ssize_t             buf_read_remain(buf_t *buf);
unsigned char*      buf_write_ptr(buf_t *buf);
unsigned char*      bug_read_ptr(buf_t *buf);
int                 buf_is_empty(buf_t *buf);
int                 buf_is_full(buf_t *buf);

void                buf_advance(buf_t *buf, ssize_t len);
void                buf_truncate(buf_t *buf, ssize_t len);
                    
int                 buf_write(buf_t *buf, unsigned char *bytes, ssize_t len);
int                 buf_write_uint64(buf_t *buf, uint64_t v);
int                 buf_write_uint32(buf_t *buf, uint32_t v);
int                 buf_write_uint16(buf_t *buf, uint16_t v);
int                 buf_write_int64(buf_t *buf, int64_t v);
int                 buf_write_int32(buf_t *buf, int32_t v);
int                 buf_write_int16(buf_t *buf, int16_t v);
int                 buf_write_byte(buf_t *buf, unsigned char v);
int 				buf_write_float(buf_t *buf, float v);
int 				buf_write_double(buf_t *buf, double v);
                    
int                 buf_read(buf_t *buf, unsigned char *out, ssize_t len);
uint64_t            buf_read_uint64(buf_t *buf);
uint32_t            buf_read_uint32(buf_t *buf);
uint16_t            buf_read_uint16(buf_t *buf);
int64_t             buf_read_int64(buf_t *buf);
int32_t             buf_read_int32(buf_t *buf);
int16_t             buf_read_int16(buf_t *buf);
unsigned char       buf_read_byte(buf_t *buf);
float 				buf_read_float(buf_t *buf);
double 				buf_read_double(buf_t *buf);

#endif