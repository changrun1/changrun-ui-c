#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <wchar.h>
#include <locale.h>
#include "ui.h"





HANDLE hConsole;




Component* init(){

    setlocale(LC_ALL, "utf-8");
    system("chcp 65001 > nul");
    init_screen();
    init_input();
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    return screen.root;
}

void set_text_format(int format){
    SetConsoleTextAttribute(hConsole, format);
    
}







// 隱藏光標
void hide_cursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// 顯示光標
void show_cursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}




void clear(){

    screen.root->children_size = 0;
    screen.component_count = 0;
    screen.focus = NULL;
    clear_input();
    
    update_screen_component();
    
    free_trash();
    
    printf("\033[H\033[J");
    clear_buffer();
    clear_format_buffer();
    clear_front_buffer();
    

}



void refresh(){
    
    hide_cursor();
    for(int i = 0; i < screen.root->height; i++){

        for(int j = 0; j < screen.root->width; j++){
            if(get_component_by_position(i, j) == NULL){
                continue;
            }
            int color_format = get_component_by_position(i, j)->color_format;

            set_text_format(get_component_by_position(i, j)->color_format);
            int format =  screen.format_buffer[i][j];

            if(strcmp(screen.buffer[i][j], screen.front_buffer[i][j]) == 0 
            && screen.color_format_buffer[i][j] == color_format 
            && screen.format_buffer[i][j] == screen.front_format_buffer[i][j]){
                continue;
            }
            
            


            printf("\033[%d;%dH", i + 1, j + 1);
            if(format == UNDERLINE){
                printf("\033[4m");
            }
            if(strncmp(screen.buffer[i][j], " ", 1) == 0 )//  || strncmp(screen.buffer[i][j], "\0", 1) == 0
            {
  
                printf(" ");

            }
            else
            {

                printf("%s", screen.buffer[i][j]);

            }
            if(format == UNDERLINE){
                printf("\033[24m");
            }

            screen.color_format_buffer[i][j] = color_format;
            screen.front_format_buffer[i][j] = format;
        }
    }
    sync_buffer();
    if(screen.focus != NULL){
        if(screen.focus->type == INPUT_FIELD){
            int x = screen.focus->abs_x + ((Text_input_data*)screen.focus->data)->count;
            int y = screen.focus->abs_y;
            show_cursor();
            printf("\033[%d;%dH", y + 1, x + 1);
        }
    }
}



void update(){
    update_input();
    refresh();
}





