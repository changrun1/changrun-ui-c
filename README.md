# UI函式庫文檔

## 整體用法

這份文檔介紹了如何使用自製的 UI 函式庫來設計基於終端的介面。

### 初始化與主迴圈

在使用 UI 函式庫時，首先需要初始化視窗並建立主迴圈以處理事件和渲染元件。以下是簡單的初始化與主迴圈範例：

```c
#include "ui_library.h"

int main() {
    // 初始化視窗
    Component* root = init();

    // 建立一個簡單的按鈕元件
    Component* button = create_button(2, 2, 10, "確定", root);

    // 主迴圈
    while (1) {
        // 檢查按鈕是否被點擊
        if (get_button_click(button)) {
            printf("按鈕被點擊！\n");
        }

        // 更新事件和渲染元件
        update();
    }


    clear();
    return 0;
}
```

在此範例中，`init()` 用於初始化根視窗，並用`update()` 更新事件和渲染元件。

---

## 元件介紹

### 1. 視窗 (Window)

視窗是根元件，通過 `init()` 函式創建。所有其他元件都需以視窗為父元件。

#### 範例

```c
Component* root = init();
```

---

### 2. 盒子 (Box)

盒子是用於分隔和排列其他元件的容器，可以新增子元件。

#### 函式

- `create_box(int x, int y, int width, int height, Component* parent)`
- `create_box_with_title(int x, int y, int width, int height, const char* title, Component* parent)`

#### 範例

```c
Component* box = create_box(0, 0, 50, 16, root);
Component* titled_box = create_box_with_title(0, 0, 50, 16, "範例盒子", root);
```

---

### 3. 按鈕 (Button)

按鈕用於觸發特定操作，默認高度為 3 格(上下框線和內容文字)，自帶框線。

#### 函式

- `create_button(int x, int y, int width, const char* label, Component* parent)`
- `get_button_click(Component* button)`

#### 範例

```c
Component* button = create_button(1, 7, 10, " 確定", box);
if (get_button_click(button)) {
    // 按鈕被點擊時執行的邏輯
}
```

---

### 4. 輸入框 (Input Field)

輸入框用於文字輸入。

#### 函式

- `create_input_field(int x, int y, int width, Component* parent)`
- `get_input_field_input(Component* input)`

#### 範例

```c
Component* input = create_input_field(7, 3, 15, box);
char* text = get_input_field_input(input);
```

---

### 5. 密碼框 (Password Field)

密碼框功能與輸入框類似，但輸入內容以 `*` 顯示。

#### 函式

- `create_password_field(int x, int y, int width, Component* parent)`

#### 範例

```c
Component* password = create_password_field(7, 5, 15, box);
char* password_text = get_input_field_input(password);
```

---

### 6. 單選框 (Radio Button)

單選框允許用戶在多個選項中選擇一個。

#### 函式

- `create_radio_button(int x, int y, int width, Component* parent)`
- `add_radio_button(Component* radio, const char* label)`

#### 範例

```c
Component* radio = create_radio_button(1, 3, 10, box);
add_radio_button(radio, "選項1");
add_radio_button(radio, "選項2");
```

---
### 7. 文字 (text)

頁面用於分隔內容，分頁器用於切換頁面。

#### 函式

- `create_text(int x, int y, const char* text, Component* parent)`
- `set_text(Component* text, const char* new_text)`

#### 範例

```c
Component* text = create_text(3, 3, "Hello, World!", box);
set_text(text, "New text");
```

---
### 8. 多選框 (Checkbox)

多選框允許用戶選擇多個選項。

#### 函式

- `create_checkbox(int x, int y, int width, const char* label, Component* parent)`

#### 範例

```c
Component* checkbox = create_checkbox(1, 3, 10, "選項1", box);
Checkbox_data* checkbox_data = (Checkbox_data*)checkbox->data;
if (checkbox_data->is_checked) {
    // 選項被選中時的邏輯
}
```

---

### 9. 下拉式選單 (Dropdown Menu)

下拉式選單用於顯示多個選項並允許用戶選擇其一。

#### 函式

- `create_dropdown_menu(int x, int y, int width, const char* default_text, Component* parent)`
- `add_dropdown_menu(Component* dropdown, const char* label)`

#### 範例

```c
Component* dropdown = create_dropdown_menu(1, 3, 17, "請選擇", box);
add_dropdown_menu(dropdown, "選項1");
add_dropdown_menu(dropdown, "選項2");
Dropdown_menu_data* dropdown_data = (Dropdown_menu_data*)dropdown->data;
char* selected = dropdown_data->text[dropdown_data->selected];
```

---

### 10. 分割線 (Bar)

就是分割線

#### 函式

- `create_bar(int x, int y, int width, Component* parent)`

#### 範例

```c
Component* bar = create_bar(3, 3, 25, box);
```

---

### 11. 分頁器與頁面 (Pager and Page)

頁面用於分隔內容，分頁器用於切換頁面。

#### 函式

- `create_page(int x, int y, int width, int height, Component* parent)`
- `create_sub_page(int x, int y, Component* parent)`
- `create_pager(int x, int y, int width, Component* parent)`

#### 範例

```c
Component* page = create_page(3, 3, 25, 10, box);

Component* sub_page1 = create_sub_page(0, 0, page);
create_text(1, 5, "頁面1", sub_page1);

Component* sub_page2 = create_sub_page(0, 0, page);
create_text(1, 5, "頁面2", sub_page2);

create_pager(3, 12, 10, page);
```

---
### 12. 文字 (text)

頁面用於分隔內容，分頁器用於切換頁面。

#### 函式

- `create_text(int x, int y, const char* text, Component* parent)`
- `set_text(Component* text, const char* new_text)`

#### 範例

```c
Component* text = create_text(3, 3, "Hello, World!", box);
set_text(text, "New text");
```

---
以上為各元件的基本用法，具體細節請參考範例程式碼及函式庫內部實現。

請注意以下幾點，這些特性可能比較不直觀：

redraw\_all\_components 不只是處理渲染：執行該函式後，元件才會真正被新增到螢幕上，否則不會顯示。

clear 不只是清空畫面：它會清除所有元件並釋放其記憶體空間，除了根元件 (root) 外，因此執行 clear 後請勿再訪問已清除的元件。

子元素的限制：只有視窗、盒子、有標題的盒子、子頁面和頁面可以新增子元素。

頁面限制：頁面僅能新增分頁器和子頁面作為子元素。

避免直接訪問 Component 的成員：請使用提供的接口函式，這樣可以避免更新渲染時出現奇怪的問題。

非阻塞主迴圈的建議：若阻塞主迴圈，輸入可能無法即時更新，視窗大小改變也不會即時重繪，因此盡量使用非阻塞的方式。

密碼框的特性：密碼框預設僅接受英文和數字輸入，並以 \* 顯示輸入內容，這些特性與一般輸入框不同。

按鈕默認高度與樣式：按鈕默認高度為 3 格，並自帶框線。

以下介紹各種元件的用法及範例程式碼。

