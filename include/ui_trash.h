#ifndef UI_TRASH_H
#define UI_TRASH_H

#include <stdlib.h>

typedef struct {
    void** trash;
    int size;
    int count;
} Trash_can;

extern Trash_can trash_can;

void init_trash_can();

void add_to_trash(void* ptr);

void free_trash();

#endif
