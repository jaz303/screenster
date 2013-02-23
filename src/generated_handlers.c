#define READ_FIXED_LENGTH(c_type, reader_type) \
    ({ \
        if (buf_read_remain(&msg->args) < sizeof(c_type)) { \
            goto param_error; \
        } \
        \
        buf_read_##reader_type(&msg->args); \
    })
    
#define READ_STRING() \
    ({ \
        if (buf_read_remain(&msg->args) < sizeof(uint16_t)) { \
            goto param_error; \
        } \
        \
        uint16_t string_length = buf_read_uint16(&msg->args); \
        if (buf_read_remain(&msg->args) < string_length + 1) { \
            goto param_error; \
        } \
        \
        char *str = HANDLER_ALLOC(string_length + 1); \
        if (str) { \
            buf_read(&msg->args, str, string_length + 1); \
            str[string_length] = 0; \
        } else { \
            fprintf(stderr, "couldn't allocate space for string param (sz=%d)\n", string_length + 1); \
            goto param_error; \
        } \
        \
        str; \
    })

#include "generated_handlers/all.c"
