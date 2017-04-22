#include "arraylist.h"
#include <errno.h>

/**
 * @visibility HIDDEN FROM USER
 * @return     true on success, false on failure
 */
static bool resize_al(arraylist_t* self){
    bool ret = false;

    return ret;
}

arraylist_t *new_al(size_t item_size){
    void *ret = NULL;

    return ret;
}

size_t insert_al(arraylist_t *self, void* data){
    size_t ret = UINT_MAX;

    resize_al(self);

    return ret;
}

size_t get_data_al(arraylist_t *self, void *data){
    size_t ret = 0;

    return ret;
}

void *get_index_al(arraylist_t *self, size_t index){
    void *ret = NULL;

    return ret;
}

bool remove_data_al(arraylist_t *self, void *data){
    bool ret = false;

    resize_al(self);

    return ret;
}

void *remove_index_al(arraylist_t *self, size_t index){
    void *ret = 0;

    resize_al(self);

    return ret;
}

void delete_al(arraylist_t *self, void (*free_item_func)(void*)){

    return;
}
