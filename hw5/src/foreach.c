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
    thread_value *value;
    pthread_key_create(&thread_key,NULL);
    (void) pthread_once(&key_once, make_key);
    if ((value = pthread_getspecific(thread_key)) == NULL) {
        value = malloc(sizeof(thread_value));
        (void) pthread_setspecific(thread_key, value);
        value->index = 0;
        value->t = self;
        sem_init(&(self->for_mutex),0,1);
        self->foreachCnt = 0;
        printf("once:\n");

    }
    printf("foreach ini:\n");
    sem_wait(&self->for_mutex);
    self->foreachCnt++;
    if (self->foreachCnt==1){
        sem_wait(&self->d);
    }
    sem_post(&self->for_mutex);

    void *ret = self->base + (value->index)*self->item_size;
    printf("end of ini:\n");
    // sem_wait(&self->for_mutex);
    // self->foreachCnt++;
    // if (self->foreachCnt==0){
    //     sem_post(&self->d);
    // }
    // sem_post(&self->for_mutex);
    return ret;
}

void *foreach_next(arraylist_t *self, void* data){
    // sem_wait(&self->for_mutex);
    // self->foreachCnt++;
    // if (self->foreachCnt==1){
    //     sem_wait(&self->insert);
    // }
    // sem_post(&self->for_mutex);
    printf("start next:\n");
    size_t index = foreach_index();
    if (index <= self->length){
        if (data!=NULL){
            memcpy(self->base + index*self->item_size,data,self->item_size);
        }
        thread_value *value = pthread_getspecific(thread_key);
        value->index++;
        void *ret = calloc(1,self->item_size);
        memcpy(ret,self->base + index*self->item_size,self->item_size);
        // sem_wait(&self->for_mutex);
        // self->foreachCnt++;
        // if (self->foreachCnt==0){
        //     sem_post(&self->insert);
        // }
        // sem_post(&self->for_mutex);
        printf("end next:\n");

        return ret;
    }
    else{
        // sem_wait(&self->for_mutex);
        // self->foreachCnt++;
        // if (self->foreachCnt==0){
        //     sem_post(&self->insert);
        // }
        // sem_post(&self->for_mutex);
        printf("end of loop:\n");
        foreach_break_f();
        return NULL;
    }
}

size_t foreach_index(){
    thread_value *v = pthread_getspecific(thread_key);
    size_t ret = v->index;
    return ret;
}

bool foreach_break_f(){
    printf("start break:\n");
    thread_value *v = pthread_getspecific(thread_key);
    arraylist_t *self = v->t;
    sem_wait(&self->for_mutex);
    self->foreachCnt++;
    if (self->foreachCnt==0){
        sem_post(&self->d);
    }
    sem_post(&self->for_mutex);
    free(v);
    printf("end of break:\n");
    return true;
}

int32_t apply(arraylist_t *self, int32_t (*application)(void*)){
    int32_t ret = 0;
    foreach(void,value,self){
        application(&value);
    }
    return ret;
}
