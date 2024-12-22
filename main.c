#include <ui.h>

void page1(Component* root){
    clear();
    Component* box = create_box(0, 0, 50, 16, root);
    create_text(1, 1, "UI函式庫測試", box);

    create_text(1, 3, "輸入1:", box);
    create_text(1, 5, "輸入2:", box);
    Component* input = create_input_field(7, 3, 15, box);
    Component* input2 = create_input_field(7, 5, 15, box);
    Component* button = create_button(1, 7, 10, " 確定", box);
    Component* text3 = create_text(1, 11, "", box);
    Component* text4 = create_text(1, 12, "", box);
    Component* box2 = create_box(27, 1, 20, 14, box);
    Component* button1 = create_button(1, 4, 10, " 加一", box2);
    Component* button2 = create_button(1, 8, 10, " 離開", box2);

    Component* text5 = create_text(1, 12, "要打什麼", box2);
    redraw_all_components();
    int count = 0;
    while(1){
        update();
        if(get_button_click(button1)){
            count++;
            char temp[100];
            sprintf(temp, "你按了 %d 次", count);
            set_text(text5, temp);
        }
        if(get_button_click(button2)){
            break;
        }
        if(get_button_click(button)){
            char* input_text = get_input_field_input(input);
            char* input_text2 = get_input_field_input(input2);
            char temp[100];
            char temp2[100];
            sprintf(temp, "輸入1: %s", input_text);
            sprintf(temp2, "輸入2: %s", input_text2);
            set_text(text3, temp);
            set_text(text4, temp2);
        }

    }
}

void test_password(Component* root){
    clear();
    Component* box = create_box_with_title(0, 0, 50, 16, "密碼輸入框測試", root);
    create_text(1, 5, "輸入1:", box);
    create_text(1, 7, "輸入2:", box);
    Component* input = create_input_field(7, 5, 15, box);
    Component* password = create_password_field(7, 7, 15, box);
    Component* button = create_button(1, 9, 10, " 確定", box);
    Component* text3 = create_text(1, 13, "", box);
    Component* text4 = create_text(1, 14, "", box);
    redraw_all_components();
    while(1){
        update();
        if(get_button_click(button)){
            char* input_text = get_input_field_input(input);
            char* input_text2 = get_input_field_input(password);
            char temp[100];
            char temp2[100];
            sprintf(temp, "輸入1: %s", input_text);
            sprintf(temp2, "輸入2: %s", input_text2);
            set_text(text3, temp);
            set_text(text4, temp2);
            

        }
        

    }

}

void test_radio(Component* root){
    clear();
    Component* box = create_box(0, 0, 50, 16, root);
    create_text(1, 1, "測試單選框", box);
    Component* radio = create_radio_button(1, 3, 10, box);
    add_radio_button(radio, "選項1");
    add_radio_button(radio, "選項2");
    add_radio_button(radio, "選項3");
    add_radio_button(radio, "選項4");
    Component* button = create_button(1, 7, 10, " 確定", box);
    Component* text = create_text(1, 11, "", box);
    redraw_all_components();
    while(1){
        update();
        if(get_button_click(button)){
            Radio_button_data* radio_data = (Radio_button_data*)radio->data;
            char temp[100];
            sprintf(temp, "選擇: %s", radio_data->text[radio_data->selected]);
            set_text(text, temp);
        }

    }
}

void test_check_button(Component* root){
    clear();
    Component* box = create_box(0, 0, 50, 16, root);
    create_text(1, 1, "測試多選框", box);
    Component* checkbox = create_checkbox(1, 3, 10, "選項1", box);
    Component* checkbox2 = create_checkbox(1, 4, 10, "選項2", box);
    Component* checkbox3 = create_checkbox(1, 5, 10, "選項3", box);
    Component* button = create_button(1, 7, 10, " 確定", box);
    Component* text = create_text(1, 11, "", box);
    redraw_all_components();
    while(1){
        update();
        if(get_button_click(button)){
            Checkbox_data* checkbox_data = (Checkbox_data*)checkbox->data;
            Checkbox_data* checkbox_data2 = (Checkbox_data*)checkbox2->data;
            Checkbox_data* checkbox_data3 = (Checkbox_data*)checkbox3->data;
            char temp[100];
            sprintf(temp, "選擇: %d %d %d", checkbox_data->is_checked, checkbox_data2->is_checked, checkbox_data3->is_checked);
            set_text(text, temp);
        }
    }
}

void test_dropdown_menu(Component* root){
    clear();
    
    Component* box = create_box(0, 0, 50, 16, root);
    create_text(1, 1, "測試下拉選單", box);
    set_corner(box, 0);

    Component* dropdown = create_dropdown_menu(1, 3, 17,"請選擇", box);

    add_dropdown_menu(dropdown, "選項1");
    add_dropdown_menu(dropdown, "離開(選項2)");
    add_dropdown_menu(dropdown, "選項3");
    add_dropdown_menu(dropdown, "選項4");
    Component* button = create_button(1, 7, 10, " 確定", box);
    Component* text = create_text(1, 11, "", box);
    redraw_all_components();
    while(1){
        
        update();
        if(get_button_click(button)){
            Dropdown_menu_data* dropdown_data = (Dropdown_menu_data*)dropdown->data;
            char temp[100];
            sprintf(temp, "選擇: %s", dropdown_data->text[dropdown_data->selected]);
            set_text(text, temp);
            if(dropdown_data->selected == 1){
                break;
            }
        }
    }
}

void test_bar(Component* root){
    clear();
    Component* box = create_box(0, 0, 50, 16, root);
    create_bar(3, 3, 25, box);
    redraw_all_components();
    while(1){
        update();
    }
}

void test_page(Component* root){
    clear();
    Component* box = create_box(0, 0, 50, 20, root);
    Component* page = create_page(3, 3, 25, 10, box);
    
    Component* sub_page = create_sub_page(0, 0, page);
    create_text(1, 1, "子頁1", sub_page);
    Component* checkbox = create_checkbox(1, 3, 10, "選項1", sub_page);
    Component* checkbox2 = create_checkbox(1, 4, 10, "選項2", sub_page);
    Component* checkbox3 = create_checkbox(1, 5, 10, "選項3", sub_page);
    Component* button = create_button(1, 7, 10, " 確定", sub_page);
    Component* text = create_text(1, 11, "", sub_page);

    
    Component* sub_page2 = create_sub_page(0, 0, page);
    create_text(1, 1, "子頁2", sub_page2);
    create_text(1, 3, "輸入1:", sub_page2);
    Component* input = create_input_field(7, 3, 15, sub_page2);
    create_text(1, 5, "輸入2:", sub_page2);
    Component* input2 = create_password_field(7, 5, 15, sub_page2);
    



    create_pager(3, 12, 10, page);
    


    redraw_all_components();
    while(1){
        update();
    }
    
}


int main(){
    
    Component* root = init();
    
    test_page(root);

    test_bar(root);

    test_dropdown_menu(root);

    test_password(root);
    
    test_radio(root);

    test_check_button(root);
    


    page1(root);
    

}