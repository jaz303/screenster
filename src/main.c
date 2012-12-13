#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>

#include "rbuf.h"

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
// Screens

#define MAX_SCREENS 15
ALLEGRO_DISPLAY* screens[MAX_SCREENS+1] = {0};

//
// Handlers

#define HANDLER_ARGS size_t cmd_len, rbuf_t *cmd
#define HANDLER_FN(name) void name(HANDLER_ARGS)

#define READ_UINT64()   rbuf_read_uint64(cmd)
#define READ_UINT32()   rbuf_read_uint32(cmd)
#define READ_UINT16()   rbuf_read_uint16(cmd)
#define READ_BYTE()     rbuf_read_byte(cmd)

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
    
    handlers_init();
    
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
