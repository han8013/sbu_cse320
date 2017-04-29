#include "debug.h"
#include "arraylist.h"
#include "foreach.h"
#include <pthread.h>
#include <string.h>


static pthread_key_t thread_key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

static void
make_key(){
    (void) pthread_key_create(&thread_key, NULL);
}

void *foreach_init(arraylist_t *self){
    /* check system call return */
    sem_wait(&self->for_mutex);
    self->foreachCnt++;
    if (self->foreachCnt==1){
        sem_wait(&self->d);
    }
    sem_post(&self->for_mutex);

    thread_value *value;
    (void) pthread_once(&key_once, make_key);
    if ((value = pthread_getspecific(thread_key)) == NULL) {
        value = malloc(sizeof(thread_value));
        value->index = 0;
        value->t = self;
        (void) pthread_setspecific(thread_key, value);
    }
    void *ret = self->base + (value->index)*self->item_size;
    return ret;
}

void *foreach_next(arraylist_t *self, void* data){

    size_t index = foreach_index();
        if (data!=NULL){
            memcpy(self->base + index*self->item_size,data,self->item_size);
        }
        thread_value *value = pthread_getspecific(thread_key);
        value->index++;
        void *ret;

        ret = calloc(1,self->item_size);
        memcpy(ret,self->base + value->index*self->item_size,self->item_size);
        (void) pthread_setspecific(thread_key, value);

    if (value->index >= self->length){
        foreach_break_f();
        return NULL;
    }
    return ret;

}

size_t foreach_index(){
    thread_value *value;
    value = pthread_getspecific(thread_key);
    size_t ret = value->index;
    return ret;
}

bool foreach_break_f(){
    thread_value *v = pthread_getspecific(thread_key);
    arraylist_t *self = v->t;
    sem_wait(&self->for_mutex);
    self->foreachCnt--;
    if (self->foreachCnt==0){
        sem_post(&self->d);
    }
    sem_post(&self->for_mutex);
    free(v);
    return true;
}

int32_t apply(arraylist_t *self, int32_t (*application)(void*)){
    int32_t ret = 0;
    foreach(void,value,self){
        application(&value);
    }
    return ret;
}
