#include "ui_screen.h"


Screen screen;

void get_window_size(int* w, int* h){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    *w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void force_set_buffer(int x, int y, int level, const char* text){
    if(x < 0 || y < 0 || x >= MAX_BUFFER_SIZE || y >= MAX_BUFFER_SIZE){
        return;
    }
    int len = utf8_char_length(text);
    if (len < 1) len = 1;
    
    strncpy(screen.buffer[x][y], text, len);
    screen.buffer[x][y][len] = '\0';
    screen.level[x][y] = level;
}


void clear_buffer(){
    for(int i = 0; i < MAX_BUFFER_SIZE; i++){
        for(int j = 0; j < MAX_BUFFER_SIZE; j++){
            force_set_buffer(i, j, -1, " ");
        }
    }

}

void clear_format_buffer(){
    for(int i = 0; i < MAX_BUFFER_SIZE; i++){
        for(int j = 0; j < MAX_BUFFER_SIZE; j++){
            screen.format_buffer[i][j] = 0;

        }
    }
}

void claer_front_format_buffer(){
    for(int i = 0; i < MAX_BUFFER_SIZE; i++){
        for(int j = 0; j < MAX_BUFFER_SIZE; j++){
            screen.front_format_buffer[i][j] = 0;
        }
    }
}

void clear_front_buffer(){
    for(int i = 0; i < MAX_BUFFER_SIZE; i++){
        for(int j = 0; j < MAX_BUFFER_SIZE; j++){
            screen.front_buffer[i][j][0] = '\0';
        }
    }
}



void handle_window_change() {

    clear_buffer();
    clear_front_buffer();
    screen.root->abs_x = 0;
    screen.root->abs_y = 0;
    get_window_size(&screen.root->width, &screen.root->height);
    redraw_all_components();
}



void init_screen(){
    screen.root = (Component*)malloc(sizeof(Component));
    screen.root->x = 0;
    screen.root->y = 0;
    screen.root->abs_x = 0;
    screen.root->abs_y = 0;
    get_window_size(&screen.root->width, &screen.root->height);
   
    screen.root->level = 0;
    screen.root->number = 0;

    screen.root->color_format = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    screen.root->type = WINDOW;
    screen.root->data = NULL;
    screen.root->parent = NULL;
    screen.root->draw = draw_window;
    screen.root->children = (Component**)malloc(sizeof(Component*) * MAX_CHILDREN);
    screen.root->children_size = 0;
    screen.focus = NULL;
    screen.component_count = 0;
    for(int i = 0; i < MAX_BUFFER_SIZE; i++){
        for(int j = 0; j < MAX_BUFFER_SIZE; j++){
            screen.screen_component[i][j] = -1;
        }
    }
   
}

void set_buffer(int x, int y, int level, const char* text){

    if(x < 0 || y < 0 || x >= MAX_BUFFER_SIZE || y >= MAX_BUFFER_SIZE){
        return;
    }
    if(level < screen.level[x][y]){
        return;
    }

    int len = utf8_char_length(text);
    if (len < 1) len = 1;
    
    strncpy(screen.buffer[x][y], text, len);
    screen.buffer[x][y][len] = '\0';
    screen.level[x][y] = level;
    screen.format_buffer[x][y] = 0;
}


void sync_buffer(){
    for(int i = 0; i < MAX_BUFFER_SIZE; i++){
        for(int j = 0; j < MAX_BUFFER_SIZE; j++){
            strncpy(screen.front_buffer[i][j], screen.buffer[i][j], 5);
            
        }
    }
}

void update_screen_component(){

    for(int i = 0; i < MAX_BUFFER_SIZE; i++){
        for(int j = 0; j < MAX_BUFFER_SIZE; j++){
            screen.screen_component[i][j] = -1;
        }
    }
    Component* components[MAX_COMPONENTS];

    int component_count = 0;
    components[component_count++] = screen.root;
    
    for (int i = 0; i < component_count; i++) {
        Component* current = components[i];
        for (int j = 0; j < current->children_size; j++) {
            components[component_count++] = current->children[j];
        }
    }
    for(int i = 0; i < component_count; i++){
        screen.component_list[i] = components[i];
    }
    screen.component_count = component_count;
    for(int i = 0; i < screen.root->height; i++){
        for(int j = 0; j < screen.root->width; j++){
            for (int k = 0; k < component_count; k++) {

                Component* current = components[k];

                if (i >= current->abs_y && i < current->abs_y + current->height && j >= current->abs_x && j < current->abs_x + current->width ) {
                    if(get_component_by_position(i, j) == NULL || get_component_by_position(i, j)->level < current->level){
                        screen.screen_component[i][j] = k;
                        
                    }
                }
            }

            
        }
    }

}



void set_buffer_and_format(int x, int y,int level, const char* text ,  int format){
    if(x < 0 || y < 0 || x >= MAX_BUFFER_SIZE || y >= MAX_BUFFER_SIZE){
        return;
    }
    if(level < screen.level[x][y]){
        return;
    }
    set_buffer(x, y, level, text);
    screen.format_buffer[x][y] = format;

}

void debug_screen_component(){
    system("cls");
    printf("\n");
    for(int i = 0; i < screen.root->height; i++){
        for(int j = 0; j < screen.root->width; j++){
            if(get_component_by_position(i, j) == NULL){
                printf("n");
            } else {
                printf("%d", get_component_by_position(i, j)->type);
            }
        }
        printf("\n");
    }
    //printf("[debug] debug_screen_component\n");
    Sleep(1000000);
}

Component* get_component_by_position(int x, int y){
    if (x < 0 || y < 0 || x >= MAX_BUFFER_SIZE || y >= MAX_BUFFER_SIZE)
    {
        return NULL;
    }
    
    if(screen.screen_component[x][y] == -1 || screen.screen_component[x][y] >= screen.component_count){
        return NULL;
    }else{
        
        return screen.component_list[screen.screen_component[x][y]];
    }
}

void focus_component(Component* component){
    if (screen.focus != NULL)
    {
        switch (screen.focus->type)
        {
            case INPUT_FIELD:
            case PASSWORD_FIELD:
                clear_input();
                screen.focus->color_format = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                break;
            case DROPDOWN_MENU:
                /*clear_buffer();
                
                ((Dropdown_menu_data*)screen.focus->data)->is_expanded = 0;*/
                //component->height = 1;

                collapse_dropdown_menu(screen.focus);
                break;

        }
    }
    
    
    if(component != NULL){
        switch(component->type){
            case INPUT_FIELD:
                
            case PASSWORD_FIELD:
                component->input_state = INPUT_CONTINUE;
                component->color_format = BACKGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                break;
            case DROPDOWN_MENU:

                /*((Dropdown_menu_data*)component->data)->is_expanded = 1;
                component->height = ((Dropdown_menu_data*)component->data)->dropdown_count + 1;*/
                expand_dropdown_menu(component);
                break;
        }
    }
    
    screen.focus = component;
    redraw_all_components();
    
}