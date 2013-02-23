#include "screenster.h"

status_t msg_pool_init(msg_pool_t *pool) {
    
    if (pthread_mutex_init(&pool->lock, NULL) != 0) {
        return GEN_ERROR;
    }
    
    pool->free = NULL;
    pool->all = NULL;
    
    return OK;

}

void msg_pool_cleanup(msg_pool_t *pool) {
    
    pthread_mutex_destroy(&pool->lock);
    
    msg_t *curr = pool->all;
    while (curr) {
        msg_t *tmp = curr->next;
        free(curr);
        curr = tmp;
    }
    
}

msg_t* msg_pool_checkout(msg_pool_t *pool) {
    
    pthread_mutex_lock(&pool->lock);
    
    msg_t *msg = pool->free;
    
    if (msg) {
        pool->free = msg->next_free;
    } else {
        msg = malloc(sizeof(msg_t) + MAX_MSG_SIZE);
        if (msg) {
            msg->next_free = NULL;
            msg->next = pool->all;
            pool->all = msg;
        }
    }
    
    pthread_mutex_unlock(&pool->lock);
    
    if (msg) {
        msg->parse_state = PARSE_OUT;
        buf_reset(&msg->args);
    }
    
    return msg;
    
}

void msg_pool_return(msg_pool_t *pool, msg_t *msg) {
    pthread_mutex_lock(&pool->lock);
    msg->next_free = pool->free;
    pool->free = msg;
    pthread_mutex_unlock(&pool->lock);
}
