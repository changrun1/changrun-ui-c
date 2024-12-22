#include "ui_trash.h"


Trash_can trash_can;

void init_trash_can(){
    trash_can.count = 0;
    if(trash_can.trash != NULL){
        free(trash_can.trash);
    }
    trash_can.trash = (void**)malloc(sizeof(void*) * 100);
    trash_can.size = 100;
}

void add_to_trash(void* ptr){
    if(trash_can.count >= trash_can.size){
        trash_can.size *= 2;
        trash_can.trash = (void**)realloc(trash_can.trash, sizeof(void*) * trash_can.size);

    }
    trash_can.trash[trash_can.count] = ptr;
    trash_can.count++;
}

void free_trash(){
    for(int i = 0; i < trash_can.count; i++){
        free(trash_can.trash[i]);
    }
    free(trash_can.trash);
    trash_can.trash = NULL;
    init_trash_can();

}