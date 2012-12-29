#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "rbuf.h"
#include "mpool.h"

const unsigned short listen_port = 32000;

#define RAW_BUFFER_SIZE      8192
#define CMD_BUFFER_SIZE     65536

unsigned char raw_buffer[RAW_BUFFER_SIZE];

enum { PARSE_OUT, PARSE_GOT_HEADER } parse_state = PARSE_OUT;

rbuf_t      *cmd_buffer;
uint8_t     cmd_category;
uint8_t     cmd_command;
uint16_t    cmd_len;

//
// Memory pool for use by command handlers
// Any allocated memory will be released after the handler returns

#define HANDLER_ALLOC(sz)       mpool_alloc(handler_pool, sz)
#define HANDLER_POOL_DRAIN()    mpool_clear(handler_pool)

#define HANDLER_POOL_SIZE 65536
mpool_t *handler_pool;

//
// Screens

#define MAX_SCREENS 15
ALLEGRO_DISPLAY* screens[MAX_SCREENS+1] = {0};
int active_screen = 0;

//
// Primitives

enum {
    DRAW_PEN_ON = 1,
    DRAW_FILL_ON = 2
};

typedef struct {
    unsigned int    flags;
    ALLEGRO_COLOR   pen_color;
    ALLEGRO_COLOR   fill_color;
    float           x;
    float           y;
} draw_state_t;

draw_state_t draw_state;

//
// General draw state

typedef struct {
    draw_state_t        draw_state;
} saved_graphics_state_t;

#define GRAPHICS_STACK_SIZE 32
saved_graphics_state_t  graphics_stack[GRAPHICS_STACK_SIZE];
int                     graphics_stack_count = 0;

void reset_draw_state() {
    draw_state.flags = DRAW_PEN_ON | DRAW_FILL_ON;
    draw_state.pen_color = al_map_rgb(255, 255, 255);
    draw_state.fill_color = al_map_rgb(0, 0, 0);
    draw_state.x = 0.0f;
    draw_state.y = 0.0f;
}

void save_graphics_state() {
    graphics_stack[graphics_stack_count].draw_state = draw_state;
    graphics_stack_count++;
}

void restore_graphics_state() {
    graphics_stack_count--;
    draw_state = graphics_stack[graphics_stack_count].draw_state;
}

//
// Handlers

#define HANDLER_ARGS size_t cmd_len, rbuf_t *cmd
#define HANDLER_FN(name) void name(HANDLER_ARGS)

#define READ_FIXED_LENGTH(c_type, reader_type) \
    ({ \
        if (rbuf_remain(cmd) < sizeof(c_type)) { \
            goto param_error; \
        } \
        \
        rbuf_read_##reader_type(cmd); \
    })
    
#define READ_STRING() \
    ({ \
        if (rbuf_remain(cmd) < sizeof(uint16_t)) { \
            goto param_error; \
        } \
        \
        uint16_t string_length = rbuf_read_uint16(cmd); \
        if (rbuf_remain(cmd) < string_length + 1) { \
            goto param_error; \
        } \
        \
        char *str = HANDLER_ALLOC(string_length + 1); \
        if (str) { \
            rbuf_read(cmd, str, string_length + 1); \
            str[string_length] = 0; \
        } else { \
            fprintf(stderr, "couldn't allocate space for string param (sz=%d)\n", string_length + 1); \
            goto param_error; \
        } \
        \
        str; \
    })

#define COLOR_TO_ALLEGRO(argb) \
    al_map_rgba( \
        ((argb) >> 16) & 0xFF, \
        ((argb) >>  8) & 0xFF, \
        ((argb) >>  0) & 0xFF, \
        ((argb) >> 24) & 0xFF \
    )


typedef void (*handler_f)(HANDLER_ARGS);
handler_f*      handler_lookup[256] = {0};
int             handler_counts[256] = {-1};

void create_category(unsigned char category, int function_count) {
    handler_lookup[category] = malloc(sizeof(handler_f) * (function_count + 1));
    handler_counts[category] = function_count;
}

void install_handler(unsigned char category, unsigned char offset, handler_f fn) {
    handler_lookup[category][offset] = fn;
}

//
// Auto-generated code

#include "handlers.c"

//
//

int create_listener() {
    
    int sock_listen = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(listen_port);
    
    bind(sock_listen, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(sock_listen, 0);
    
    printf("listening on port %d...\n", listen_port);
    
    return sock_listen;

}

int process() {
    
    while (1) {
        switch (parse_state) {
            case PARSE_OUT:
                if (rbuf_count(cmd_buffer) >= 4) {
                    cmd_category    = rbuf_read_byte(cmd_buffer);
                    cmd_command     = rbuf_read_byte(cmd_buffer);
                    cmd_len         = rbuf_read_uint16(cmd_buffer);
                    parse_state     = PARSE_GOT_HEADER;
                } else {
                    goto done;
                }
                break;
            case PARSE_GOT_HEADER:
                if (rbuf_count(cmd_buffer) >= cmd_len) {
                    if (cmd_command == 0 || cmd_command > handler_counts[cmd_category]) {
                        fprintf(stderr, "unknown command: [0x%02x:0x%02x]\n", cmd_category, cmd_command);
                        // TODO: handle error
                    } else {
                        ssize_t expected_count_after = rbuf_count(cmd_buffer) - cmd_len;
                        handler_lookup[cmd_category][cmd_command](cmd_len, cmd_buffer);
                        HANDLER_POOL_DRAIN();
                        while (rbuf_count(cmd_buffer) > expected_count_after) {
                            rbuf_read_byte(cmd_buffer);
                        }
                        if (rbuf_count(cmd_buffer) < expected_count_after) {
                            fprintf(stderr, "error: command handler for [0x%02x:0x%02x] consumed too much buffer! (disconnecting...)\n", cmd_category, cmd_command);
                            return 0;
                        }
                    }
                    parse_state = PARSE_OUT;
                } else {
                    goto done;
                }
                break;
        }
    }
    
done:
    return 1;
}

int main(int argc, char *argv[]) {
    
    cmd_buffer = rbuf_create(CMD_BUFFER_SIZE);
    if (!cmd_buffer) {
        fprintf(stderr, "could not allocate input buffer\n");
        return 1;
    }
    
    handler_pool = mpool_create(HANDLER_POOL_SIZE);
    if (!handler_pool) {
        fprintf(stderr, "could not allocate handler memory pool\n");
        return 1;
    }
    
    handlers_init();

    //
    // Initialise Allegro

    al_init();
    al_init_primitives_addon();

    reset_draw_state();

    //
    //
    
    int listener_socket = create_listener();
    
    while (1) {
        
        struct sockaddr_in  client_addr;
        socklen_t           client_addr_len     = sizeof(client_addr);
        int                 conn                = accept(listener_socket,
                                                         (struct sockaddr*)&client_addr,
                                                         &client_addr_len);
        
        printf("new connection accepted\n");
        
        rbuf_clear(cmd_buffer);
        parse_state = PARSE_OUT;
        
        while (1) {
            
            ssize_t read = recvfrom(conn,
                                    raw_buffer,
                                    RAW_BUFFER_SIZE,
                                    0,
                                    (struct sockaddr*)&client_addr,
                                    &client_addr_len);
            
            if (read <= 0) {
                break;
            }
            
            if (!rbuf_write(cmd_buffer, raw_buffer, read)) {
                fprintf(stderr, "buffer overrun!\n");
                break;
            }
            
            if (!process()) {
                break;
            }
        
        }
        
        close(conn);
        printf("connection closed\n");
    
    }
    
    return 0;
}
