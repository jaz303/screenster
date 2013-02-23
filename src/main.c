#include "screenster.h"
#include "generated_handlers.c"

void init() {
    
    obj_init();
    register_builtin_types();
    handlers_init();
    
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    
}

int create_listener(unsigned short port) {
    
    int sock_listen = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    
    bind(sock_listen, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(sock_listen, 0);
    
    printf("listening on port %d...\n", port);
    
    return sock_listen;

}

int parse(msg_t *msg) {
    
    if (msg->parse_state == PARSE_OUT) {
        if (buf_size(&msg->args) >= 4) {
            msg->cmd_category   = buf_read_byte(&msg->args);
            msg->cmd_command    = buf_read_byte(&msg->args);
            msg->cmd_len        = buf_read_uint16(&msg->args);
            if (msg->cmd_len > MAX_MSG_ARGS_SIZE) {
                return PARSE_ARGS_TOO_LONG;
            } else {
                msg->parse_state = PARSE_GOT_HEADER;
            }
        } else {
            return PARSE_INCOMPLETE;
        }
    }
    
    if (msg->parse_state == PARSE_GOT_HEADER) {
        if (buf_size(&msg->args) >= msg->cmd_len) {
            return PARSE_COMPLETE;
        } else {
            return PARSE_INCOMPLETE;
        }
    }
    
    assert(0 /* shouldn't get here */);
    
}

void* connection_handler(void *arg) {
    
    conn_t *conn = (conn_t*)arg;
    
    msg_t *msg = msg_pool_checkout(&conn->msg_pool);
    if (!msg) {
        fprintf(stderr, "could not checkout message\n");
        goto disconnect;
    }
    
    assert(msg->args.capacity > 0);
    assert(buf_size(&msg->args) == 0);
    assert(buf_read_pos(&msg->args) == 0);
    
    while (1) {
        
        ssize_t read = recv(conn->fd, buf_write_ptr(&msg->args), buf_write_remain(&msg->args), 0);
        if (read == -1) {
            fprintf(stderr, "recv() error\n");
            goto disconnect;
        }
        
        buf_advance(&msg->args, read);
        
    reparse:
        switch (parse(msg)) {
            case PARSE_COMPLETE:
                {
                    msg_t *new_msg = msg_pool_checkout(&conn->msg_pool);
                    if (new_msg == NULL) {
                        fprintf(stderr, "chould not checkout message\n");
                        goto disconnect;
                    }
                    
                    ssize_t total_msg_len = buf_read_pos(&msg->args) + msg->cmd_len;
                    ssize_t diff = buf_size(&msg->args) - total_msg_len;
                    if (diff > 0) {
                        buf_truncate(&msg->args, total_msg_len);
                        buf_write(&new_msg->args, buf_write_ptr(&msg->args), diff);
                    }
                    
                    ALLEGRO_EVENT evt;
                    evt.user.type = SCREENSTER_EVENT_MESSAGE_RECEIVED;
                    evt.user.data1 = (intptr_t)msg;
                    
                    al_emit_user_event(&conn->event_source, &evt, NULL);
                    
                    msg = new_msg;
                    
                    if (diff > 0) {
                        goto reparse;
                    }
                }
                break;
            case PARSE_INCOMPLETE:
                /* do nothing, keep reading */
                break;
            case PARSE_ARGS_TOO_LONG:
                fprintf(stderr, "message args too long; disconnecting\n");
                goto disconnect;
        }
        
    }

    
disconnect:

    fprintf(stderr, "connection closing...\n");

    ALLEGRO_EVENT evt = { .type = SCREENSTER_EVENT_DISCONNECT };
    al_emit_user_event(&conn->event_source, &evt, NULL);

    pthread_exit(NULL);
    
}

void dispatch(conn_t *conn, msg_t *msg) {
    handler_f hnd = get_handler(msg->cmd_category, msg->cmd_command);
    if (hnd == NULL) {
        fprintf(stderr, "unknown command: [0x%02x:0x%02x]\n", msg->cmd_category, msg->cmd_command);
        // TODO: handle error
    } else {
        hnd(conn, msg);
        mpool_clear(&conn->arg_pool);
    }
}

int main(int argc, char *argv[]) {
    
    init();
    
    int listener_socket = create_listener(DEFAULT_PORT);
    while (1) {
        
        conn_t conn;
        conn.client_addr_len = sizeof(conn.client_addr);
        conn.fd = accept(listener_socket, (struct sockaddr*)&conn.client_addr, &conn.client_addr_len);
        
        int mpool_ready = 1;
        int msg_pool_ready = 1;
        ALLEGRO_EVENT_QUEUE *events = NULL;
        
        if (!mpool_init(&conn.arg_pool, HANDLER_POOL_SIZE)) {
            fprintf(stderr, "could not initialise handler pool\n");
            mpool_ready = 0;
            goto disconnected;
        }
        
        if (msg_pool_init(&conn.msg_pool) != OK) {
            fprintf(stderr, "could not initialise message pool\n");
            msg_pool_ready = 0;
            goto disconnected;
        }
        
        events = al_create_event_queue();
        if (!events) {
            goto disconnected;
        }
        
        al_init_user_event_source(&conn.event_source);
        al_register_event_source(events, &conn.event_source);
        
        printf("new connection accepted...\n");
        
        pthread_attr_t io_thread_attr;
        pthread_attr_init(&io_thread_attr);
        pthread_attr_setdetachstate(&io_thread_attr, PTHREAD_CREATE_JOINABLE);
        
        pthread_t io_thread;
        pthread_create(&io_thread, &io_thread_attr, connection_handler, &conn);
        
        pthread_attr_destroy(&io_thread_attr);
        
        for (int loop = 1; loop;) {
            ALLEGRO_EVENT evt;
            al_wait_for_event(events, &evt);
            
            switch (evt.type) {
                case ALLEGRO_EVENT_JOYSTICK_AXIS:
                    break;
                case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
                case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
                    break;
                case ALLEGRO_EVENT_KEY_DOWN:
                case ALLEGRO_EVENT_KEY_UP:
                case ALLEGRO_EVENT_KEY_CHAR:
                    break;
                case ALLEGRO_EVENT_MOUSE_AXES:
                case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
                case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    break;
                case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
                    break;
                case SCREENSTER_EVENT_MESSAGE_RECEIVED:
                    dispatch(&conn, (msg_t*)evt.user.data1);
                    msg_pool_return(&conn.msg_pool, (msg_t*)evt.user.data1);
                    break;
                case SCREENSTER_EVENT_DISCONNECT:
                    loop = 0;
                    break;
                default:
                    break;
            }
            
            if (ALLEGRO_EVENT_TYPE_IS_USER(evt.type)) {
                al_unref_user_event((ALLEGRO_USER_EVENT*)&evt);
            }
        }
        
        pthread_join(io_thread, NULL);
        
    disconnected:
    
        close(conn.fd);
        
        if (mpool_ready) {
            mpool_cleanup(&conn.arg_pool);
        }
    
        if (msg_pool_ready) {
            msg_pool_cleanup(&conn.msg_pool);
        }
        
        if (events) {
            al_destroy_user_event_source(&conn.event_source);
            al_destroy_event_queue(events);
        }
        
    }
    
    pthread_exit(NULL);
    return 0;
    
}