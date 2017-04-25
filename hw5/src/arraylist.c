#include "arraylist.h"
#include <errno.h>
#include "const.h"
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
/**
 * @visibility HIDDEN FROM USER
 * @return     true on success, false on failure
 */
static bool resize_al(arraylist_t* self){
    bool ret = false;
    size_t length = self->length;
    size_t capacity = self->capacity;
    size_t item_size = self->item_size;
    if (length == capacity){
        self->capacity = 2*capacity;
        self->base = realloc(self->base,item_size*self->capacity);
        memset(self->base+item_size*(self->capacity/2),'0',item_size*(self->capacity/2));
        return true;
    }
    else if (length == (capacity/2) - 1){
        if (capacity/2>=INIT_SZ){
            self->capacity = capacity/2;
        }
        return true;
    }
    return ret;
}

arraylist_t *new_al(size_t item_size){
    void *ret = NULL;
    if (item_size<=0){
        fprintf(stderr, "Value of errno: %d\n", errno);
        return ret;
    }
    else{
        arraylist_t *arraylist = malloc(sizeof(arraylist_t));
        arraylist->capacity = INIT_SZ;
        arraylist->length = 0;
        arraylist->item_size = item_size;
        arraylist->base = calloc(INIT_SZ,item_size);
        ret = arraylist;
        return ret;
    }
}

size_t insert_al(arraylist_t *self, void* data){
    size_t ret = UINT_MAX;
    if (self==NULL || data == NULL){
        fprintf(stderr, "errno: %d\n", errno);
        return ret;
    }
    if (self->length == self->capacity){
        resize_al(self);
    }
    void *dest = (void*)(self->base+((self->length)*(self->item_size)));
    memcpy(dest,data,self->item_size);
    ret = self->length;
    self->length++;
    // printf("insert return value check %d\n",(int)ret );
    //     test_item_t *t = (test_item_t*)((size_t)self->base+(ret*self->item_size));
    //     printf("insert first value %d\n", t->i);
    return ret;
}

size_t get_data_al(arraylist_t *self, void *data){
    size_t ret = 0;
    if (data==NULL){
        return ret;
    }
    for (int i = 0; i < self->length; ++i){
        void *temp = (void*)(self->base+(i*self->item_size));
                // test_item_t *t = (test_item_t*)(self->base+(i*self->item_size));
                // printf("insert first value %d\n", t->i);
        if (memcmp(temp,data,self->item_size)==0){
            // printf("found \n");
            ret = i;
            return ret;
        }
    }
    ret = UINT_MAX;
    return ret;
}

void *get_index_al(arraylist_t *self, size_t index){
    void *ret = NULL;
    ret = calloc(1,self->item_size);
    if (index >= self->length){
        void* sour = (self->base+((self->length-1)*self->item_size));
        memcpy(ret,sour,self->item_size);
    }
    else{
        void* sour = (self->base+index*self->item_size);

        memcpy(ret,sour,self->item_size);
        // test_item_t *t = (test_item_t*)(self->base+(index*self->item_size));
        // printf("index %d\n", (int)index);
        // printf("insert value test4------------------------ %d\n", t->i);
        // test_item_t *t1 = (test_item_t*)ret;
        // printf("insert %d\n", t1->i);
    }
    return ret;
}

bool remove_data_al(arraylist_t *self, void *data){
    bool ret = false;
    if (self->length ==0 ){
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
        ret = true;
        return ret;
    }
    return ret;
}

void *remove_index_al(arraylist_t *self, size_t index){
    void *ret = 0;
    void *base = self->base;
    size_t item_size = self->item_size;
    if (index >= self->length){
        void* sour = (void*)(base+self->length*item_size);
        ret = calloc(1,item_size);
        memcpy(ret,sour,item_size);
    }
    else{
        void *sour = (void*)(base+index*item_size);
        ret = calloc(1,item_size);
        memcpy(ret,sour,item_size);
        for (int j = index+1; j < self->length; ++j){
            void* dest = (void*)(base+(j-1)*item_size);
            void* sour = (void*)(base+j*item_size);
            memmove(dest,sour,item_size);
        }
    }
    self->length--;
    if (self->length == (self->capacity/2) - 1){
        resize_al(self);
    }

    return ret;
}

void delete_al(arraylist_t *self, void (*free_item_func)(void*)){
    return;
}
