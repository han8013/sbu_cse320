#include "arraylist.h"
#include <errno.h>
#include "const.h"
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include "csapp.h"

int errno;
static bool resize_al(arraylist_t* self){
    bool ret = false;
    size_t length = self->length;
    size_t capacity = self->capacity;
    size_t item_size = self->item_size;
    if (length == capacity){
        self->capacity = 2*capacity;
        void *temp = calloc(length, item_size);
        memcpy(temp,self->base,item_size*length);
        free(self->base);
        self->base = calloc(self->capacity, item_size);
        memcpy(self->base,temp,item_size*length);
        free(temp);
        ret = true;
    }
    else if (length == (capacity/2) - 1){
        if (capacity/2>=INIT_SZ){
            self->capacity = capacity/2;
        }
        ret = true;
    }
    return ret;
}

arraylist_t *new_al(size_t item_size){
    void *ret = NULL;
    if (item_size<=0){
        errno = ENOMEM;
        return ret;
    }
    else{
        arraylist_t *arraylist = malloc(sizeof(arraylist_t));
        arraylist->capacity = INIT_SZ;
        arraylist->length = 0;
        arraylist->item_size = item_size;
        arraylist->base = calloc(INIT_SZ,item_size);
        ret = arraylist;
        Sem_init(&(arraylist->mutex),0,1);
        Sem_init(&(arraylist->w),0,1);
        Sem_init(&(arraylist->d),0,1);
        Sem_init(&(arraylist->for_mutex),0,1);
        arraylist->foreachCnt = 0;
        arraylist->readcnt = 0;
        return ret;
    }
}

size_t insert_al(arraylist_t *self, void* data){
    size_t ret = UINT_MAX;
    if (self==NULL || data == NULL){
        errno = ENOMEM;
        return ret;
    }
    P(&(self->w));
    if (self->length == self->capacity){
        resize_al(self);
    }
    void *dest = (void*)(self->base+((self->length)*(self->item_size)));
    memcpy(dest,data,self->item_size);
    ret = self->length;
    self->length++;
    V(&(self->w));
    return ret;
}

size_t get_data_al(arraylist_t *self, void *data){
    size_t ret = UINT_MAX;
    if (data==NULL){
        errno=EINVAL;
        return ret;
    }
    P(&(self->mutex));
    self->readcnt++;
    if (self->readcnt==1){
        P(&(self->d));
        P(&(self->w));
    }
    V(&(self->mutex));
    ret = UINT_MAX;
    for (int i = 0; i < self->length; ++i){
        void *temp = (void*)(self->base+(i*self->item_size));
        if (memcmp(temp,data,self->item_size)==0){
            ret = i;
        }
    }
    P(&self->mutex);
    self->readcnt--;
    if (self->readcnt==0){
        V(&(self->w));
        V(&(self->d));

    }
    V(&(self->mutex));

    return ret;
}

void *get_index_al(arraylist_t *self, size_t index){
    void *ret = NULL;
    if (self->length == 0 ){
        errno = EINVAL;
        return ret;
    }
    P(&(self->mutex));
    self->readcnt++;
    if (self->readcnt==1){
        P(&(self->d));
        P(&(self->w));

    }
    V(&(self->mutex));
    ret = calloc(1,self->item_size);
    if (index >= self->length){
        void* sour = (self->base+((self->length-1)*self->item_size));
        memcpy(ret,sour,self->item_size);
    }
    else{
        void* sour = (self->base+index*self->item_size);
        memcpy(ret,sour,self->item_size);
    }

    P(&(self->mutex));
    self->readcnt--;
    if (self->readcnt==0){
        V(&(self->w));
        V(&(self->d));
    }
    V(&(self->mutex));

    return ret;
}

bool remove_data_al(arraylist_t *self, void *data){
    P(&(self->d));
    bool ret = false;
    if (self->length ==0 ){
        errno = EINVAL;
        return ret;
    }
    else{
        void *base = self->base;
        size_t item_size = self->item_size;
        if (data == NULL){
            /* revove first one */
            for (int i = 1; i < self->length; ++i){
                void* dest = (void*)(base+(i-1)*item_size);
                void* sour = (void*)(base+i*item_size);
                memmove(dest,sour,item_size);
            }
        }
        else{
            int j;
            for (int i = 0; i < self->length; ++i){
                void *temp = (void*)(base+i*item_size);
                if (memcmp(temp,data,self->item_size)==0){
                    j = i;
                    ret = true;
                }
            }
            for (j = 1; j < self->length; ++j){
                void* dest = (void*)(base+(j-1)*item_size);
                void* sour = (void*)(base+j*item_size);
                memmove(dest,sour,item_size);
            }
        }
        self->length--;
        if (self->length == (self->capacity/2) - 1){
            resize_al(self);
        }
        // ret = true;
    }
    V(&(self->d));
    return ret;
}

void *remove_index_al(arraylist_t *self, size_t index){
    P(&(self->d));
    if (self->length==0){
        errno = EINVAL;
        return NULL;
    }
    void *ret = 0;
    void *base = self->base;
    size_t item_size = self->item_size;
    if (index >= self->length){
        void* sour = (void*)(base+(self->length-1)*item_size);
        ret = calloc(1,item_size);
        memcpy(ret,sour,item_size);
    }
    else{

        void *sour = (void*)(base+index*item_size);
        ret = calloc(1,item_size);
        memcpy(ret,sour,item_size);
        for (int j = index; j < self->length-1; ++j){
            void* dest = (void*)(base+j*item_size);
            void* sour = (void*)(base+(j+1)*item_size);
            memmove(dest,sour,item_size);
        }
    }
    self->length--;
    if (self->length == (self->capacity/2) - 1){
        resize_al(self);
    }

    V(&(self->d));
    return ret;
}

void delete_al(arraylist_t *self, void (*free_item_func)(void*)){
    P(&(self->d));
    if (free_item_func!=NULL && self!=NULL){
        for (int i = 0; i < self->length; ++i){
            void *t = self->base+i*self->item_size;
            (*free_item_func)(t);
        }
    }
    else{
        errno = EINVAL;
    }
    V(&(self->d));
}

