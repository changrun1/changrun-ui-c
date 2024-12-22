#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H



#include "ui_trash.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define MAX_INPUT_SIZE 256
#define MAX_CHILDREN 100


enum component_type {
    WINDOW,
    TEXT,
    TEXT_AREA, // 還沒有
    BOX,
    BOX_WITH_TITLE,
    BUTTON,
    INPUT_FIELD,
    PASSWORD_FIELD,
    TABLE, // 還沒有
    CELL, // 還沒有
    CHECKBOX,
    RADIO_BUTTON,
    DROPDOWN_MENU,
    DATE_PICKER, // 還沒有
    BAR,
    PAGE, // 還沒有
    SUB_PAGE, // 還沒有
    PAGER, // 還沒有
    NAVIGATION_BAR // 還沒有


};

typedef enum {
    INPUT_CONTINUE,
    INPUT_COMPLETE,
    NO_INPUT,
    INPUT_ERROR
} Input_state;

typedef struct Component {
    int type;
    int x;
    int y;
    int abs_x;
    int abs_y;
    int width;
    int height;
    int level;
    int number;
    int color_format;
    int format;
    int round_corner;
    Input_state input_state;
    void* data;
    void (*draw)(struct Component*);
    struct Component* parent;
    struct Component** children;
    int children_size;
} Component;

#define DEFAULT_FORMAT 0 // format
#define UNDERLINE 1 // format

typedef enum {
    NUMBER = 1,
    ENGLISH = 2,
    SYMBOL = 4,
    CHINESE = 8,
    ALL_INPUT_TYPE = (NUMBER | ENGLISH | SYMBOL | CHINESE)
} Input_type;

typedef struct {
    char* text;
    int length; // 字數
    int total_size; // 總大小
    int size[MAX_INPUT_SIZE * 100];   // 空間
    int length_max;
    int count;
    Input_type type;
} Text_input_data;

typedef struct {
    int is_clicked;
    char* text;
} Button_data;


typedef struct {
    int is_checked;
    char* text;
} Checkbox_data;

typedef struct {
    int radio_button_count;
    int selected;
    char* text[MAX_CHILDREN];
} Radio_button_data;

typedef struct {
    int dropdown_count;
    int selected;
    int is_expanded;
    char* default_text;
    char* text[MAX_CHILDREN];
} Dropdown_menu_data;



typedef struct {
    int sub_page_count;
    int current_page;
    Component** sub_pages;


} Page_data;

Component* create_box(int x, int y, int width, int height, Component* parent);
Component* create_text(int x, int y, char* text, Component* parent);
Component* create_input_field(int x, int y, int width, Component* parent);
Component* create_button(int x, int y, int width, char* text, Component* parent);
Component* create_box_with_title(int x, int y, int width, int height, char* title, Component* parent);
Component* create_checkbox(int x, int y, int width, char* text, Component* parent);
Component* create_password_field(int x, int y, int width, Component* parent);
Component* create_radio_button(int x, int y, int width, Component* parent);
Component* create_dropdown_menu(int x, int y, int width,char* default_text, Component* parent);
Component* create_bar(int x, int y, int width, Component* parent);
Component* create_page(int x, int y, int width, int height, Component* parent);
Component* create_sub_page(int x, int y, Component* parent);
Component* create_pager(int x, int y, int width, Component* parent);


void set_component_format(Component* component, int format);
void set_text(Component* textComponent, char* text);
void set_corner(Component* component, int round_corner);
void add_radio_button(Component* radio_button, char* text);
void add_dropdown_menu(Component* dropdown_menu, char* text);
void page_to(Component* page, int index);
void expand_dropdown_menu(Component* dropdown_menu);
void collapse_dropdown_menu(Component* dropdown_menu);

int get_button_click(Component* button);
int get_checkbox_checked(Component* checkbox);
char* get_input_field_input(Component* textComponent);


void draw_box(Component* box);
void draw_text(Component* text);
void draw_button(Component* button);
void draw_window(Component* window);
void draw_radio_button(Component* radio_button);
void draw_bar(Component* bar);
void draw_input_field(Component* input_field);
void draw_password_field(Component* password_field);
void draw_box_with_title(Component* box);
void draw_dropdown_menu(Component* dropdown_menu);
void draw_checkbox(Component* checkbox);
void draw_page(Component* page);
void draw_sub_page(Component* sub_page);
void draw_pager(Component* pager);
void redraw_all_components();
void debug_input_data(Text_input_data* data);
void debug_component(Component* component);

#include "ui_screen.h"

#endif // UI_COMPONENTS_H