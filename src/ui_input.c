#include "ui_input.h"
#include <windows.h>
#include <ctype.h>

#include "tools.h"

Input_state handle_text_input(Text_input_data *text_data);

HANDLE hInput;

void start_input_thread();

void init_input()
{
    setlocale(LC_ALL, "utf-8");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    hInput = GetStdHandle(STD_INPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(hInput, &mode);

    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT);

    start_input_thread();
}

struct Input_thread_data
{
    char buffer[128][4];
    int buffer_size;
    int text_is_input_available;
    struct Event *event;
    int event_count;
} Input_thread_data;

HANDLE input_thread_mutex;

struct Event get_event()
{
    WaitForSingleObject(input_thread_mutex, INFINITE);
    if (Input_thread_data.event_count == 0)
    {
        ReleaseMutex(input_thread_mutex);
        return (struct Event){EVENT_NONE, 0, 0, 0, NULL};
    }
    if (Input_thread_data.event != NULL)
    {
        struct Event event = *Input_thread_data.event;
        free(Input_thread_data.event);
        Input_thread_data.event = event.next;
        ReleaseMutex(input_thread_mutex);
        return event;
    }
    else
    {
        ReleaseMutex(input_thread_mutex);
        return (struct Event){EVENT_NONE, 0, 0, 0, NULL};
    }
}

void add_event(struct Event event)
{
    WaitForSingleObject(input_thread_mutex, INFINITE);
    struct Event *newEvent = (struct Event *)malloc(sizeof(struct Event));
    *newEvent = event;
    newEvent->next = Input_thread_data.event;
    Input_thread_data.event = newEvent;
    Input_thread_data.event_count++;
    ReleaseMutex(input_thread_mutex);
}

int is_text_input_available()
{
    WaitForSingleObject(input_thread_mutex, INFINITE);
    int result = Input_thread_data.text_is_input_available;
    ReleaseMutex(input_thread_mutex);
    return result;
}

void update_input()
{
    struct Event event = get_event();
    while (event.type != EVENT_NONE)
    {
        if (event.type == ON_CLICK)
        {
            int x = event.x;
            int y = event.y;
            Component *component = get_component_by_position(y, x);
            if (component != NULL)
            {
                
                focus_component(component);
                switch (component->type)
                {
                case INPUT_FIELD:

                    break;
                case BUTTON:

                    ((Button_data *)component->data)->is_clicked = 1;
                    break;
                case RADIO_BUTTON:
                {
                    Radio_button_data *radio_data = (Radio_button_data *)component->data;
                    int selected = y - component->abs_y;
                    if (selected >= 0 && selected < radio_data->radio_button_count)
                    {
                        radio_data->selected = selected;
                    }
                    draw_radio_button(component);
                    break;
                }
                case CHECKBOX:
                {
                    Checkbox_data *checkbox_data = (Checkbox_data *)component->data;
                    checkbox_data->is_checked = !checkbox_data->is_checked;
                    draw_checkbox(component);
                    break;
                }
                case DROPDOWN_MENU:
                {
                    Dropdown_menu_data *dropdown_data = (Dropdown_menu_data *)component->data;
                    if (dropdown_data->is_expanded)
                    {
                        int selected = y - component->abs_y - 1;
                        if (selected >= 0 && selected < dropdown_data->dropdown_count)
                        {
                            dropdown_data->selected = selected;
                            collapse_dropdown_menu(component);
                            if (screen.focus == component)
                            {
                                focus_component(NULL);
                            }
                        }
                    }

                    break;
                }
                case PAGER:
                {
                    Component *page = component->parent;
                    Page_data *page_data = (Page_data *)page->data;
                    int abs_x = x - component->abs_x;
                    
                    if (abs_x >= 0 && abs_x < component->width) {

                        if(abs_x >= 0 && abs_x < 2){
                            
                            page_to(page, page_data->current_page - 1);
                        }
                        else if(abs_x >= component->width - 2 && abs_x < component->width){
                            /*printf("[debug] to page %d\n", page_data->current_page + 1);
                            system("pause");*/

                            page_to(page, page_data->current_page + 1);
                        }
                    }
                   
                    
                    break;
                }
                }
            }
        }
        else if (event.type == KEY_DOWN)
        {
        }
        else if (event.type == WINDOW_RESIZE)
        {

            handle_window_change();
        }
        event = get_event();
    }
    if (screen.focus == NULL)
        return;
    switch (screen.focus->type)
    {
    case INPUT_FIELD:
        
        if (is_text_input_available())
        {

            //debug_input_data((Text_input_data *)screen.focus->data);
            Input_state state = handle_text_input((Text_input_data *)screen.focus->data);
            screen.focus->input_state = state;
            if (state == INPUT_COMPLETE)
            {
                focus_component(NULL);
            }
        }

        break;
    case PASSWORD_FIELD:
        
        if (is_text_input_available())
        {
            //debug_input_data((Text_input_data *)screen.focus->data);
            Input_state state = handle_text_input((Text_input_data *)screen.focus->data);
            screen.focus->input_state = state;
            if (state == INPUT_COMPLETE)
            {
                focus_component(NULL);
            }
        }
        break;
    }
}

DWORD WINAPI console_input_thread(LPVOID lpParam)
{
    (void)lpParam;
    INPUT_RECORD inputRecord;
    DWORD events;

    while (1)
    {

        ReadConsoleInputW(hInput, &inputRecord, 1, &events);

        if (inputRecord.EventType == MOUSE_EVENT)
        {

            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
            if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            {

                int x = mouseEvent.dwMousePosition.X;
                int y = mouseEvent.dwMousePosition.Y;
                add_event((struct Event){ON_CLICK, x, y, 0, NULL});
            }
        }
        if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
        {

            wchar_t c = inputRecord.Event.KeyEvent.uChar.UnicodeChar;

            if (c == 0)
            {
                add_event((struct Event){KEY_DOWN, 0, 0, inputRecord.Event.KeyEvent.wVirtualKeyCode, NULL});
                continue;
            }

            char utf8Buffer[4] = {0};

            int bytesWritten = WideCharToMultiByte(CP_UTF8, 0, &c, 1, utf8Buffer, 4, NULL, NULL);

            if (bytesWritten == 0)
            {
                continue;
            }

            WaitForSingleObject(input_thread_mutex, INFINITE);

            for (int i = 0; i < bytesWritten; i++)
            {
                Input_thread_data.buffer[Input_thread_data.buffer_size][i] = utf8Buffer[i];
            }

            Input_thread_data.text_is_input_available = 1;

            Input_thread_data.buffer_size++;
            if (Input_thread_data.buffer_size >= 128)
            {
                printf("[debug] buffer full\n");
                system("pause");
                exit(1);
            }
            ReleaseMutex(input_thread_mutex);
        }
        if (inputRecord.EventType == WINDOW_BUFFER_SIZE_EVENT)
        {

            add_event((struct Event){WINDOW_RESIZE, 0, 0, 0, NULL});
        }
    }
}

void start_input_thread()
{
    input_thread_mutex = CreateMutex(NULL, FALSE, NULL);
    Input_thread_data.buffer_size = 0;
    Input_thread_data.text_is_input_available = 0;
    Input_thread_data.event = NULL;
    Input_thread_data.event_count = 0;
    CreateThread(NULL, 0, console_input_thread, NULL, 0, NULL);
}

int get_text_input(char *buffer)
{

    if (Input_thread_data.buffer_size == 0)
    {
        return 0;
    }
    WaitForSingleObject(input_thread_mutex, INFINITE);

    for (int i = 0; i < 4; i++)
    {
        buffer[i] = Input_thread_data.buffer[0][i];
    }
    for (int i = 0; i < Input_thread_data.buffer_size - 1; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Input_thread_data.buffer[i][j] = Input_thread_data.buffer[i + 1][j];
        }
    }
    Input_thread_data.buffer_size--;
    if (Input_thread_data.buffer_size == 0)
    {
        Input_thread_data.text_is_input_available = 0;
    }

    ReleaseMutex(input_thread_mutex);
    return 1;
}

Input_state handle_text_input(Text_input_data *text_data)
{

    char utf8Buffer[4] = {0};
    if (get_text_input(utf8Buffer) == 0)
    {
        return NO_INPUT;
    }

    char ch = utf8Buffer[0];
    if (ch == '\b')
    {
        if (text_data->length > 0)
        {
            if (text_data->size[text_data->length - 1] == 3)
            {
                text_data->count -= 2;
            }
            else
            {
                text_data->count--;
            }
            for (int i = 0; i < text_data->size[text_data->length - 1]; i++)
            {
                text_data->total_size--;
                text_data->text[text_data->total_size] = '\0';
            }
            text_data->length--;
            redraw_all_components();
            return INPUT_CONTINUE;
        }

        redraw_all_components();
        return INPUT_CONTINUE;
    }
    else if (ch == '\n' || ch == '\r')
    {
        redraw_all_components();
        return INPUT_COMPLETE;
    }
    else if ((unsigned char)ch == 0xE0 || (unsigned char)ch == 0xE1 || (unsigned char)ch == 0xE2 || (unsigned char)ch == 0xE3)
    {

        return NO_INPUT;
    }
    else
    {
        if (text_data->length_max <= text_data->count)
            return INPUT_ERROR;
        int len = utf8_char_length(&ch);
        if (len < 1)
            len = 1;
        int type = text_data->type;
        int is_valid = 0;
        if (type & NUMBER)
        {
            is_valid = isdigit(ch);
        }
        if (type & ENGLISH)
        {
            is_valid = is_valid || isalpha(ch);
        }
        if (type & SYMBOL)
        {
            is_valid = is_valid || ispunct(ch);
        }
        if (type & CHINESE)
        {
            is_valid = is_valid || ((len == 3) && (unsigned char)ch >= 0xE0);
        }
        if (!is_valid)
        {

            return NO_INPUT;
        }

        text_data->size[text_data->length] = len;
        text_data->text[text_data->total_size++] = ch;
        for (int i = 1; i < len; i++)
        {
            text_data->text[text_data->total_size++] = utf8Buffer[i];
        }
        text_data->text[text_data->total_size] = '\0';
        text_data->length++;
        if (len == 3)
        {
            text_data->count += 2;
        }
        else
        {
            text_data->count++;
        }
        redraw_all_components();
        return INPUT_CONTINUE;
    }

    return NO_INPUT;
}

void clear_input()
{
    WaitForSingleObject(input_thread_mutex, INFINITE);
    Input_thread_data.buffer_size = 0;
    Input_thread_data.text_is_input_available = 0;
    while (Input_thread_data.event != NULL)
    {
        struct Event *event = Input_thread_data.event;
        Input_thread_data.event = event->next;
        free(event);
    }
    Input_thread_data.event_count = 0;
    ReleaseMutex(input_thread_mutex);
}
