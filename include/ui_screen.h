#ifndef UI_SCREEN_H
#define UI_SCREEN_H

#define MAX_BUFFER_SIZE 1000
#define MAX_COMPONENTS 100

#include "ui_components.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <tools.h>
#include <ui_input.h>



typedef struct {
    char buffer[MAX_BUFFER_SIZE][MAX_BUFFER_SIZE][6];
    char front_buffer[MAX_BUFFER_SIZE][MAX_BUFFER_SIZE][6];
    int color_format_buffer[MAX_BUFFER_SIZE][MAX_BUFFER_SIZE];
    int screen_component[MAX_BUFFER_SIZE][MAX_BUFFER_SIZE];
    int level[MAX_BUFFER_SIZE][MAX_BUFFER_SIZE];
    int format_buffer[MAX_BUFFER_SIZE][MAX_BUFFER_SIZE];
    int front_format_buffer[MAX_BUFFER_SIZE][MAX_BUFFER_SIZE];
    Component* component_list[MAX_COMPONENTS];
    int component_count;
    Component* focus;
    Component* root;
} Screen;

extern Screen screen;

void init_screen();
void set_buffer(int x, int y, int level, const char* text);
void set_buffer_and_format(int x, int y,int level, const char* text ,  int format);
void sync_buffer();
void clear_buffer();
void clear_front_buffer();
void clear_format_buffer();
void claer_front_format_buffer();
void handle_window_change();
void update_screen_component();
void debug_screen_component();
Component* get_component_by_position(int x, int y);
void focus_component(Component* component);

#endif