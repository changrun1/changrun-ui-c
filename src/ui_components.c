#include "ui_components.h"
#include <windows.h>
#include "tools.h"

void debug_component(Component *component)
{
    if (component == NULL)
    {
        return;
    }
    COORD coord;
    coord.X = 0;
    coord.Y = 20;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("[debug] =====================================\n");
    printf("[debug] x = %d\n", component->x);
    printf("[debug] y = %d\n", component->y);
    printf("[debug] width = %d\n", component->width);
    printf("[debug] height = %d\n", component->height);
    printf("[debug] level = %d\n", component->level);
    printf("[debug] number = %d\n", component->number);
    printf("[debug] type = %d\n", component->type);
    printf("[debug] parent = %p\n", component->parent);
    printf("[debug] children = %p\n", component->children);
    printf("[debug] =====================================\n");
}

void debug_input_data(Text_input_data *data)
{
    COORD coord;
    coord.X = 0;
    coord.Y = 20;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("[debug] =====================================\n");
    printf("[debug] text = %s\n", data->text);
    printf("[debug] length = %d\n", data->length);
    printf("[debug] total_size = %d\n", data->total_size);
    printf("[debug] length_max = %d\n", data->length_max);
    printf("[debug] =====================================\n");
}

Component *get_standard_component(int x, int y, Component *parent)
{
    if(parent->type != BOX 
    && parent->type != BOX_WITH_TITLE 
    && parent->type != SUB_PAGE 
    && parent->type != PAGE
    && parent->type != WINDOW){
        return NULL;
    }
    if(parent->children_size >= MAX_CHILDREN){
        return NULL;
    }
    
    Component *component = (Component *)malloc(sizeof(Component));
    component->x = x;
    component->y = y;
    component->abs_x = parent->abs_x + x;
    component->abs_y = parent->abs_y + y;
    component->width = 0;
    component->height = 0;
    component->format = DEFAULT_FORMAT;
    component->draw = NULL;
    //component->format = UNDERLINE;
    component->parent = parent;
    component->children = NULL;
    component->children_size = 0;
    component->level = parent->level + 2;
    component->number = parent->children_size;
    component->color_format = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    parent->children[parent->children_size] = component;
    parent->children_size++;
    return component;
}

Component *create_box(int x, int y, int width, int height, Component *parent)
{

    Component *box = get_standard_component(x, y, parent);
    
    box->width = width;
    box->height = height;
    box->type = BOX;
    box->draw = draw_box;
    box->round_corner = 1;
    box->children = (Component **)malloc(sizeof(Component *) * MAX_CHILDREN);
    box->children_size = 0;
    add_to_trash(box->children);
    add_to_trash(box);
    update_screen_component();
    return box;
}

Component *create_box_with_title(int x, int y, int width, int height, char *title, Component *parent)
{
    Component *box_with_title = create_box(x, y, width, height, parent);
    box_with_title->type = BOX_WITH_TITLE;
    box_with_title->draw = draw_box_with_title;
    char *title_data = (char *)malloc(sizeof(char) * strlen(title) * 4);

    strncpy(title_data, title, strlen(title));
    title_data[strlen(title)] = '\0';
    box_with_title->data = title_data;
    add_to_trash(title_data);
    update_screen_component();

    return box_with_title;
}

Component *create_text(int x, int y, char *text, Component *parent)
{
    Component *text_component = get_standard_component(x, y, parent);
    text_component->height = 1;
    text_component->type = TEXT;
    text_component->draw = draw_text;
    // text_component->data = text;
    int len = strlen(text);
    char *text_data = (char *)malloc(sizeof(char) * len * 4);
    strncpy(text_data, text, len);
    text_data[len] = '\0';
    text_component->data = text_data;
    add_to_trash(text_data);
    add_to_trash(text_component);
    update_screen_component();
    return text_component;
}

Component *create_input_field(int x, int y, int width, Component *parent)
{
    Component *input_field = get_standard_component(x, y, parent);

    input_field->width = width;
    input_field->height = 1;
    input_field->type = INPUT_FIELD;

    input_field->data = (Text_input_data *)malloc(sizeof(Text_input_data));
    input_field->input_state = NO_INPUT;
    input_field->draw = draw_input_field;
    ((Text_input_data *)input_field->data)->text = (char *)malloc(sizeof(char) * MAX_INPUT_SIZE * 4);
    ((Text_input_data *)input_field->data)->text[0] = '\0';
    ((Text_input_data *)input_field->data)->length = 0;
    ((Text_input_data *)input_field->data)->count = 0;
    ((Text_input_data *)input_field->data)->total_size = 0;
    ((Text_input_data *)input_field->data)->length_max = width;
    ((Text_input_data *)input_field->data)->type = ALL_INPUT_TYPE;

    add_to_trash(((Text_input_data *)input_field->data)->text);
    add_to_trash(input_field);
    add_to_trash(input_field->data);
    update_screen_component();
    return input_field;
}

Component *create_button(int x, int y, int width, char *text, Component *parent)
{
    Component *button = get_standard_component(x, y, parent);
    button->width = width;
    button->height = 3;
    button->type = BUTTON;
    button->round_corner = 1;
    button->draw = draw_button;
    char *text_data = (char *)malloc(sizeof(char) * strlen(text) * 4);
    strncpy(text_data, text, strlen(text));
    text_data[strlen(text)] = '\0';

    Button_data *button_data = (Button_data *)malloc(sizeof(Button_data));
    button_data->is_clicked = 0;
    button_data->text = text_data;
    button->data = button_data;
    add_to_trash(button_data);
    add_to_trash(button);
    add_to_trash(text_data);
    update_screen_component();
    return button;
}

Component *create_radio_button(int x, int y, int width, Component *parent)
{
    Component *radio_button = get_standard_component(x, y, parent);
    radio_button->width = width;
    radio_button->height = 0;
    radio_button->type = RADIO_BUTTON;
    radio_button->draw = draw_radio_button;
    Radio_button_data *radio_button_data = (Radio_button_data *)malloc(sizeof(Radio_button_data));
    radio_button_data->radio_button_count = 0;
    radio_button_data->selected = -1;
    radio_button->data = radio_button_data;

    add_to_trash(radio_button_data);

    add_to_trash(radio_button);

    update_screen_component();
    return radio_button;
}

Component *create_checkbox(int x, int y, int width, char *text, Component *parent)
{

    Component *checkbox = get_standard_component(x, y, parent);
    checkbox->width = width;
    checkbox->height = 1;
    checkbox->type = CHECKBOX;
    checkbox->draw = draw_checkbox;
    Checkbox_data *checkbox_data = (Checkbox_data *)malloc(sizeof(Checkbox_data));
    checkbox_data->is_checked = 0;
    char *text_data = (char *)malloc(sizeof(char) * strlen(text) * 4);
    strncpy(text_data, text, strlen(text));
    text_data[strlen(text)] = '\0';
    checkbox_data->text = text_data;

    checkbox->data = checkbox_data;
    add_to_trash(checkbox_data);
    add_to_trash(checkbox);
    add_to_trash(text_data);

    update_screen_component();
    
    return checkbox;
}

Component *create_password_field(int x, int y, int width, Component *parent)
{
    Component *password_field = create_input_field(x, y, width, parent);
    password_field->type = PASSWORD_FIELD;
    password_field->draw = draw_password_field;
    ((Text_input_data *)password_field->data)->type = NUMBER | ENGLISH | SYMBOL;
    return password_field;
}

Component *create_dropdown_menu(int x, int y, int width, char *default_text, Component *parent)
{
    Component *dropdown_menu = get_standard_component(x, y, parent);
    dropdown_menu->width = width;
    dropdown_menu->height = 1;
    dropdown_menu->type = DROPDOWN_MENU;
    Dropdown_menu_data *dropdown_menu_data = (Dropdown_menu_data *)malloc(sizeof(Dropdown_menu_data));
    dropdown_menu_data->selected = -1;
    dropdown_menu_data->dropdown_count = 0;
    dropdown_menu->data = dropdown_menu_data;
    dropdown_menu->draw = draw_dropdown_menu;
    ((Dropdown_menu_data *)dropdown_menu->data)->is_expanded = 0;
    ((Dropdown_menu_data *)dropdown_menu->data)->default_text = (char *)malloc(sizeof(char) * strlen(default_text) * 4);
    strncpy(((Dropdown_menu_data *)dropdown_menu->data)->default_text, default_text, strlen(default_text));
    ((Dropdown_menu_data *)dropdown_menu->data)->default_text[strlen(default_text)] = '\0';
    add_to_trash(((Dropdown_menu_data *)dropdown_menu->data)->default_text);
    add_to_trash(dropdown_menu_data);
    add_to_trash(dropdown_menu);
    update_screen_component();

    return dropdown_menu;
}

Component* create_bar(int x, int y, int width, Component* parent){
    Component* bar = get_standard_component(x, y, parent);
    bar->width = width;
    bar->height = 1;
    bar->type = BAR;
    bar->draw = draw_bar;
    add_to_trash(bar);
    update_screen_component();
    return bar;
}

Component* create_page(int x, int y, int width, int height, Component* parent){
    Component* page = get_standard_component(x, y, parent);
    page->width = width;
    page->height = height;
    page->type = PAGE;
    page->draw = draw_page;
    page->children_size = 0; // 只放當前子頁和分頁器
    page->children = (Component**)malloc(sizeof(Component*) * 2);
    page->children[0] = NULL;
    page->children[1] = NULL;
    Page_data* page_data = (Page_data*)malloc(sizeof(Page_data));
    
    page_data->sub_page_count = 0;
    page_data->current_page = 0;
    page_data->sub_pages = (Component**)malloc(sizeof(Component*) * MAX_CHILDREN);
    page->data = page_data;
    add_to_trash(page_data->sub_pages);
    add_to_trash(page_data);
    add_to_trash(page);
    update_screen_component();
    return page;
}

Component* create_sub_page(int x, int y, Component* parent){
    if(parent->type != PAGE){
        printf("parent is not a page\n");
        system("pause");
        return NULL;
    }
    parent->children_size = 0; // 讓他放在第一個位置
    Component* sub_page = get_standard_component(x, y, parent);
    if(parent->children[1] != NULL){
        parent->children_size = 2;
    }else{
        parent->children_size = 1;
    }
    sub_page->width = parent->width;
    sub_page->height = parent->height;
    sub_page->children = (Component**)malloc(sizeof(Component*) * MAX_CHILDREN);
    
    sub_page->type = SUB_PAGE;
    sub_page->draw = draw_sub_page;

    ((Page_data*)parent->data)->sub_pages[((Page_data*)parent->data)->sub_page_count] = sub_page;
    ((Page_data*)parent->data)->sub_page_count++;
    add_to_trash(sub_page);
    add_to_trash(sub_page->children);
    //update_screen_component();
    return sub_page;
}

// 加完子頁再加分頁器
Component* create_pager(int x, int y, int width, int height, Component* parent){
    if(parent->type != PAGE){
        printf("parent is not a page\n");
        system("pause");
        return NULL;
    }
    if(parent->children_size == 2){
        printf("parent already has a pager\n");
        system("pause");
        return NULL;
    }
    if(parent->children_size == 0){
        printf("parent has no sub page\n");
        system("pause");
        return NULL;
    }
    parent->children_size = 1; // 讓他放在第二個位置
    Component* pager = get_standard_component(x, y, parent);
    parent->children[1] = pager;
    parent->children_size = 2;
    ((Page_data*)parent->data)->current_page = 0;
    parent->children[0] = ((Page_data*)parent->data)->sub_pages[0];
    
    pager->width = width;
    pager->height = height;
    pager->type = PAGER;
    pager->draw = draw_pager;

    parent->children[1] = pager;
    add_to_trash(pager);
    update_screen_component();
    return pager;
}

void add_radio_button(Component *radio_button, char *text)
{
    if (radio_button->type != RADIO_BUTTON)
    {
        return;
    }
    Radio_button_data *radio_button_data = (Radio_button_data *)radio_button->data;
    radio_button_data->text[radio_button_data->radio_button_count] = text;
    radio_button_data->radio_button_count++;
    radio_button->height = radio_button_data->radio_button_count;
    update_screen_component();
    radio_button->draw(radio_button);
}

void add_dropdown_menu(Component *dropdown_menu, char *text)
{
    if (dropdown_menu->type != DROPDOWN_MENU)
    {
        return;
    }
    Dropdown_menu_data *dropdown_menu_data = (Dropdown_menu_data *)dropdown_menu->data;
    dropdown_menu_data->text[dropdown_menu_data->dropdown_count] = text;
    dropdown_menu_data->dropdown_count++;
    update_screen_component();
    dropdown_menu->height = dropdown_menu_data->dropdown_count + 1;
}

void set_component_format(Component *component, int format)
{
    component->color_format = format;
    update_screen_component();
    component->draw(component);
}

void set_text(Component *component, char *text)
{
    if (component->type != TEXT)
    {
        return;
    }
    int len = strlen(text);
    char *text_data = (char *)malloc(sizeof(char) * len * 4);
    strncpy(text_data, text, len);
    text_data[len] = '\0';
    component->data = text_data;
    add_to_trash(text_data);
    update_screen_component();
    component->draw(component);
}

void set_corner(Component *component, int round_corner)
{
    component->round_corner = round_corner;
    update_screen_component();
    component->draw(component);
}

void page_to(Component* page, int index){
    if(page->type != PAGE){
        return;
    }
    Page_data* page_data = (Page_data*)page->data;
    if(index < 0 || index >= page_data->sub_page_count){
        return;
    }
    page_data->current_page = index;
    page->children[0] = page_data->sub_pages[index];
    update_screen_component();
    page->draw(page);
}


void expand_dropdown_menu(Component *dropdown_menu)
{
    if (dropdown_menu->type != DROPDOWN_MENU)
    {
        return;
    }
    Dropdown_menu_data *dropdown_menu_data = (Dropdown_menu_data *)dropdown_menu->data;
    dropdown_menu_data->is_expanded = 1;
    dropdown_menu->height = dropdown_menu_data->dropdown_count + 1;
    dropdown_menu->level = dropdown_menu->parent->level + 3;
    update_screen_component();
    dropdown_menu->draw(dropdown_menu);

}

void collapse_dropdown_menu(Component *dropdown_menu)
{

    if (dropdown_menu->type != DROPDOWN_MENU)
    {
        return;
    }
    clear_buffer();
    clear_format_buffer();
    Dropdown_menu_data *dropdown_menu_data = (Dropdown_menu_data *)dropdown_menu->data;
    dropdown_menu_data->is_expanded = 0;
    dropdown_menu->height = 1;
    dropdown_menu->level = dropdown_menu->parent->level + 2;
    update_screen_component();
    dropdown_menu->draw(dropdown_menu);
}

int get_button_click(Component *button)
{
    if (button->type != BUTTON)
    {
        return 0;
    }

    int is_clicked = ((Button_data *)button->data)->is_clicked;
    ((Button_data *)button->data)->is_clicked = 0;


    return is_clicked;
}

char *get_input_field_input(Component *textComponent)
{
    if (textComponent->type != INPUT_FIELD && textComponent->type != PASSWORD_FIELD)
    {
        return NULL;
    }
    char *text = (char *)malloc(sizeof(char) * MAX_INPUT_SIZE * 4);
    strcpy(text, ((Text_input_data *)textComponent->data)->text);
    return text;
    // return ((Text_input_data *)textComponent->data)->text;
}

/*void draw(Component *Component)
{
    switch (Component->type)
    {
    case BOX:
        draw_box(Component);
        break;
    case TEXT:
        draw_text(Component);
        break;
    case WINDOW:
        break;
    case INPUT_FIELD:
        draw_input_field(Component);
        break;
    case BUTTON:
        draw_button(Component);
        break;
    case BOX_WITH_TITLE:
        draw_box_with_title(Component);
        break;
    case RADIO_BUTTON:
        draw_radio_button(Component);
        break;
    case CHECKBOX:
        draw_checkbox(Component);
        break;
    case PASSWORD_FIELD:
        draw_password_field(Component);
        break;
    case DROPDOWN_MENU:
        draw_dropdown_menu(Component);
        break;
    }
}*/

int draw_string(int x, int y,int level, char *text, int width, int format)
{
    int col = 0;
    const char *p = (const char *)text;
    
    while (*p != '\0')
    {

        if (col >= width)
        {
            break;
        }

        int len = utf8_char_length(p);

        if (len < 1)
            len = 1;


        set_buffer_and_format(y, x + col,level, p, format);
        if (len == 3)
        {
            set_buffer_and_format(y, x + col + 1,level, "\0", format);
            col += 2;

            
        }
        else
        {
            col++;
        }
        p += len;

    }

    return col;
}


void draw_window(Component *window)
{
    (void)window; // 什麼都不用做，他只負責放其他的元件
}

void draw_page(Component *page)
{
    (void)page;
}

void draw_sub_page(Component *sub_page)
{
    (void)sub_page;
}

void draw_pager(Component *pager)
{
    (void)pager;
}

void draw_dropdown_menu(Component *dropdown_menu)
{
    
    Dropdown_menu_data *dropdown_menu_data = (Dropdown_menu_data *)dropdown_menu->data;
    int level = dropdown_menu->level;
    for(int i = 0; i < dropdown_menu->width - 2; i++){
        set_buffer_and_format(dropdown_menu->abs_y, dropdown_menu->abs_x + i + 1,level, " ", UNDERLINE);
    }
    set_buffer_and_format(dropdown_menu->abs_y, dropdown_menu->abs_x + dropdown_menu->width - 3,level, "▼", UNDERLINE);
    set_buffer_and_format(dropdown_menu->abs_y, dropdown_menu->abs_x,level, "▕", DEFAULT_FORMAT);
    set_buffer_and_format(dropdown_menu->abs_y, dropdown_menu->abs_x + dropdown_menu->width - 1,level , "▏", DEFAULT_FORMAT);
    
    if (dropdown_menu_data->selected != -1)
    {
        draw_string(dropdown_menu->abs_x + 1, dropdown_menu->abs_y,level, dropdown_menu_data->text[dropdown_menu_data->selected], dropdown_menu->width - 6, UNDERLINE);
    }
    else
    {
        draw_string(dropdown_menu->abs_x + 1, dropdown_menu->abs_y,level, dropdown_menu_data->default_text, dropdown_menu->width - 6, UNDERLINE);
    }
    if (dropdown_menu_data->is_expanded)
    {

        for (int i = 0; i < dropdown_menu_data->dropdown_count; i++)
        {

            set_buffer_and_format(dropdown_menu->abs_y + i + 1, dropdown_menu->abs_x,level, "▕", DEFAULT_FORMAT);
            set_buffer_and_format(dropdown_menu->abs_y + i + 1, dropdown_menu->abs_x + dropdown_menu->width - 1,level, "▏", DEFAULT_FORMAT);
            for(int j = 0; j < dropdown_menu->width - 2; j++){
                set_buffer_and_format(dropdown_menu->abs_y + i + 1, dropdown_menu->abs_x + j + 1,level, " ", UNDERLINE);
            }
            draw_string(dropdown_menu->abs_x + 1, dropdown_menu->abs_y + i + 1,level, dropdown_menu_data->text[i], dropdown_menu->width - 6, UNDERLINE);
        }
    }
}

void draw_password_field(Component *password_field)
{
    int level = password_field->level;
    Text_input_data *password_data = (Text_input_data *)password_field->data;
    int col = 0;
    for (int i = 0; i < password_data->length; i++)
    {
        if (col >= password_field->width)
        {
            break;
        }
        //set_buffer(password_field->abs_y, password_field->abs_x + col, level, "*");
        set_buffer_and_format(password_field->abs_y, password_field->abs_x + col, level, "*", UNDERLINE);
        col++;
    }
    while (col < password_field->width)
    {
        //set_buffer(password_field->abs_y, password_field->abs_x + col, level, " ");
        set_buffer_and_format(password_field->abs_y, password_field->abs_x + col, level, " ", UNDERLINE);
        col++;
    }
}

void draw_bar(Component* bar){
    int level = bar->level;
    for(int i = 0; i < bar->width; i++){
        set_buffer(bar->abs_y, bar->abs_x + i, level, "─");
    }
}

void draw_checkbox(Component *checkbox)
{
    Checkbox_data *checkbox_data = (Checkbox_data *)checkbox->data;
    int level = checkbox->level;
    if (checkbox_data->is_checked)
    {
        set_buffer(checkbox->abs_y, checkbox->abs_x, level, "■ ");
    }
    else
    {
        set_buffer(checkbox->abs_y, checkbox->abs_x, level, "□ ");
    }
    draw_string(checkbox->abs_x + 2, checkbox->abs_y, level, checkbox_data->text, checkbox->width - 2, DEFAULT_FORMAT);
}

void draw_radio_button(Component *radio_button)
{
    Radio_button_data *radio_button_data = (Radio_button_data *)radio_button->data;
    int level = radio_button->level;
    for (int i = 0; i < radio_button_data->radio_button_count; i++)
    {
        if (radio_button_data->selected == i)
        {
            set_buffer(radio_button->abs_y + i, radio_button->abs_x, level, "●");
        }
        else
        {
            set_buffer(radio_button->abs_y + i, radio_button->abs_x, level, "○");
        }
        draw_string(radio_button->abs_x + 2, radio_button->abs_y + i, level, radio_button_data->text[i], radio_button->width - 2, DEFAULT_FORMAT);
    }
}

void draw_box_with_title(Component *box)
{
    draw_box(box);
    int level = box->level;
    draw_string(box->abs_x + 1, box->abs_y + 1, level, (char *)box->data, box->width - 2, DEFAULT_FORMAT);
    for (int i = 0; i < box->width; i++)
    {
        set_buffer(box->abs_y + 2, box->abs_x + i, level, "─");
    }
    set_buffer(box->abs_y + 2, box->abs_x, level, "├");
    set_buffer(box->abs_y + 2, box->abs_x + box->width - 1, level, "┤");
}

void draw_button(Component *button)
{
    int level = button->level;
    for (int i = 0; i < button->width; i++)
    {
        set_buffer(button->abs_y, button->abs_x + i, level, "─");
        set_buffer(button->abs_y + 2, button->abs_x + i, level, "─");
    }
    for (int i = 0; i < 3; i++)
    {
        set_buffer(button->abs_y + i, button->abs_x, level, "│");
        set_buffer(button->abs_y + i, button->abs_x + button->width - 1, level, "│");
    }
    if (button->round_corner)
    {
        set_buffer(button->abs_y, button->abs_x, level, "╭");
        set_buffer(button->abs_y, button->abs_x + button->width - 1, level, "╮");
        set_buffer(button->abs_y + 2, button->abs_x, level, "╰");
        set_buffer(button->abs_y + 2, button->abs_x + button->width - 1, level, "╯");
    }
    else
    {
        set_buffer(button->abs_y, button->abs_x, level, "┌");
        set_buffer(button->abs_y, button->abs_x + button->width - 1, level, "┐");
        set_buffer(button->abs_y + 2, button->abs_x, level, "└");
        set_buffer(button->abs_y + 2, button->abs_x + button->width - 1, level, "┘");
    }
    draw_string(button->abs_x + 2, button->abs_y + 1, level, ((Button_data *)button->data)->text, button->width, DEFAULT_FORMAT);
}

void draw_text(Component *textComponent)
{
    int level = textComponent->level;
    int col = draw_string(textComponent->abs_x, textComponent->abs_y, level, (char *)textComponent->data, textComponent->parent->width - 2, DEFAULT_FORMAT);
    textComponent->width = col;
}

void draw_input_field(Component *input_field)
{
    int level = input_field->level;
    int col = draw_string(input_field->abs_x, input_field->abs_y, level, ((Text_input_data *)input_field->data)->text, input_field->width, UNDERLINE);
    while (col < input_field->width)
    {
        //set_buffer(input_field->abs_y, input_field->abs_x + col, level, " ");
        set_buffer_and_format(input_field->abs_y, input_field->abs_x + col, level, " ", UNDERLINE);
        col++;

    }
}

void draw_box(Component *box)
{
    int level = box->level;
    for (int i = 0; i < box->width; i++)
    {
        set_buffer(box->abs_y, box->abs_x + i, level, "─");
        set_buffer(box->abs_y + box->height - 1, box->abs_x + i, level, "─");
    }
    for (int i = 0; i < box->height; i++)
    {
        set_buffer(box->abs_y + i, box->abs_x, level, "│");
        set_buffer(box->abs_y + i, box->abs_x + box->width - 1, level, "│");
    }
    if (box->round_corner)
    {
        set_buffer(box->abs_y, box->abs_x, level, "╭");
        set_buffer(box->abs_y, box->abs_x + box->width - 1, level, "╮");
        set_buffer(box->abs_y + box->height - 1, box->abs_x, level, "╰");
        set_buffer(box->abs_y + box->height - 1, box->abs_x + box->width - 1, level, "╯");
    }
    else
    {
        set_buffer(box->abs_y, box->abs_x, level, "┌");
        set_buffer(box->abs_y, box->abs_x + box->width - 1, level, "┐");
        set_buffer(box->abs_y + box->height - 1, box->abs_x, level, "└");
        set_buffer(box->abs_y + box->height - 1, box->abs_x + box->width - 1, level, "┘");
    }
}

void redraw_all_components()
{

    update_screen_component();

    for (int i = 0; i < screen.component_count; i++)
    {
        if (screen.component_list[i] == NULL)
        {
            printf("[debug] error");
            Sleep(1000000);
        }
        else
        {   
            if(screen.component_list[i]->draw != NULL){
                screen.component_list[i]->draw(screen.component_list[i]);
            }
        }
    }
}