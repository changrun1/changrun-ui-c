#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>
#include <wchar.h>
#include "ui_trash.h"
#include "ui_components.h"
#include "ui_screen.h"
#include "ui_input.h"

#define WIDTH 40
#define HEIGHT 20
#define MAX_CHILDREN 100







Component* init();

void clear();
void refresh();
void update();



#endif // UI_H