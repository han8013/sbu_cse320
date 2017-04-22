#ifndef FOREACH_H
#define FOREACH_H

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "const.h"

#define foreach(type, value, items) for ( type *value = foreach_init(items)  \
                                        ; value ;                            \
                                        value = foreach_next(items, value))

#define foreach_break if( foreach_break_f() ) break

void *foreach_init(void *self);

void *foreach_next(void *self, void* data);

size_t foreach_index();

bool foreach_break_f();

int apply(void *self, int (*application)(void*));

#endif
