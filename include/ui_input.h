#ifndef UI_INPUT_H
#define UI_INPUT_H


void clear_input();

#include <windows.h>
#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "ui_screen.h"
#include "ui_components.h"
#include "ui_trash.h"



typedef enum 
{
    EVENT_NONE = 0,
    ON_CLICK,
    MOUSE_MOVE,
    WHEEL_UP,
    WHEEL_DOWN,
    KEY_DOWN,
    WINDOW_RESIZE
} Event_type;

//typedef struct Event Event;

struct Event
{
    Event_type type;
    int x; // 滑鼠
    int y; // 滑鼠
    int key; // 鍵盤
    struct Event* next;
} ;



void init_input();
void update_input();




#endif // UI_INPUT_H