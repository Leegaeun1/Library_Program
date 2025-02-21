#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parson.h"

#define UP 72
#define DOWN 80
#define ENTER 13

void GotoXY(int x, int y) { //커서이동함수
    COORD Cur = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void HideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo); // 현재 커서 정보를 가져옴
    cursorInfo.bVisible = FALSE;                // 커서를 숨김
    SetConsoleCursorInfo(hConsole, &cursorInfo); // 변경된 커서 정보 설정
}

void Menu(); //전체 메뉴
int SelectMenu(); //메뉴 선택하기
void AddBook(JSON_Array* array, JSON_Value* rootValue); //도서 추가
void SearchBooks(JSON_Array* array, JSON_Value* rootValue); //도서 검색
void SearchAuthor(JSON_Array* array, JSON_Value* rootValue); //작가명으로 검색
void SearchBookname(JSON_Array* array, JSON_Value* rootValue); //도서명으로 검색
void Rent(JSON_Array* array, JSON_Value* rootValue); //대출/반납
void PrintBook(JSON_Array* array, JSON_Value* rootValue); // 파일에서 책 읽어오기
void RecommendBook(JSON_Array* array, JSON_Value* rootValue); //추천 도서 목록
void UpdateBook(JSON_Array* array, JSON_Value* rootValue, size_t size); //편집

int main() {
    JSON_Value* rootValue = json_parse_file("Book.json"); //파일을 읽어서 파싱
    JSON_Object* rootObject = json_value_get_object(rootValue); // object포인터 얻기
    if (rootValue == NULL) { // 파일이 없을때
        printf("도서 목록이 비어있습니다.\n");
        return 1;
    }
    JSON_Array* array = json_value_get_array(rootValue); //파싱한걸 배열로 저장
    if (array == NULL) { // 배열이 없으면 메모리 해제시켜주기
        printf("Failed to get JSON array.\n");
        json_value_free(rootValue);
        return 1;
    }
    int selectedMenu;
    HideCursor();
    while (1) {
        selectedMenu = SelectMenu();
        system("cls");
        switch (selectedMenu) {
        case 1:
            PrintBook(array, rootValue);
            break;
        case 2:
            RecommendBook(array, rootValue);
            break;
        case 3:
            AddBook(array, rootValue);
            break;
        case 4:
            SearchBooks(array, rootValue);
            break;
        case 5:
            Rent(array, rootValue);
            break;
        case 6:
            //초기화
            rootValue = json_value_init_object();             // JSON_Value 생성 및 초기화
            rootObject = json_value_get_object(rootValue);    // JSON_Value에서 JSON_Object를 얻음
            json_serialize_to_file_pretty(rootValue, "Book.json"); //사람이 읽기 쉬운 문자열로 만든뒤 파일 저장
            json_value_free(rootValue); // 메모리 해제
            array = json_value_get_array(rootValue); // 초기화된 배열 얻기
            printf("초기화 완료\n");
            break;
        case 7:
            return 0;
        }
        printf("\n 엔터를 눌러 메뉴로 돌아갑니다");
        getchar();
        system("cls");
    }
    json_value_free(rootValue);
    return 0;
}

void PrintBook(JSON_Array* array, JSON_Value* rootValue) {
    size_t array_size = json_array_get_count(array); // 배열 크기 얻기
    if (array_size == 0) { // 배열 크기가 0이면 리턴
        printf("Failed to get JSON array.\n");
        return 1;
    }
    printf("========================도서 목록=========================\n");
    for (size_t i = 0; i < array_size; i++) {
        JSON_Object* obj = json_array_get_object(array, i); // i번째 배열을 얻어옴
        if (obj != NULL) {
            const char* title = json_object_get_string(obj, "Title"); //키가 Title인것의 값을 가져옴
            const char* author = json_object_get_string(obj, "Author"); //키가 Author 인것의 값을 가져옴
            printf("    Title: %s, Author: %s\n", title, author);
            printf("----------------------------------------------------------\n");
        }
    }
    int getC = 1, mY = 1;
    int edit = 0;
    size_t size = 0;
    GotoXY(1, mY);
    printf("▶");
    while (getC != ENTER) {
        if (_kbhit()) {
            getC = _getch();
            switch (getC) {
            case UP:
                if (size > 0) {
                    GotoXY(1, mY);
                    printf("  ");
                    mY -= 2;
                    GotoXY(1, mY);
                    printf("▶");
                    size -= 2;
                }
                break;
            case DOWN:
                if (size < array_size) {
                    GotoXY(1, mY);
                    printf("  ");
                    mY += 2;
                    GotoXY(1, mY);
                    printf("▶");
                    size += 2;
                }
                break;
            case ENTER:
                system("cls");
                printf("편집을 원하시면 1, 삭제를 원하시면 2, 메뉴로 돌아가기를 원하시면 3을 입력해주세요 :");
                scanf("%d", &edit);
                getchar();

                system("cls");
                if (edit == 1) { //편집
                    system("cls");
                    UpdateBook(array, rootValue, size);
                }
                else if (edit == 2) { //삭제
                    printf("도서를 삭제합니다.\n");
                    json_array_remove(array, size / 2); //인덱스로 접근하여 삭제
                    json_serialize_to_file_pretty(rootValue, "Book.json"); //삭제한걸 저장
                    printf("도서가 삭제되었습니다!\n");
                }
                break;
            }
        }
    }
}

int SelectMenu() {
    Menu();
    int menuSelect = 1; //메뉴 선택
    int getC = 1, mY = 3;

    GotoXY(6, mY);
    printf("▶");
    while (getC != ENTER) {
        if (_kbhit()) { //키보드가 입력된 상태인가?
            getC = _getch(); //누른거 아스키코드로 받음
            switch (getC) {
            case UP:
                if (menuSelect > 1) {
                    GotoXY(6, mY);
                    printf("    ");
                    mY -= 2;
                    GotoXY(6, mY);
                    printf("▶");
                    menuSelect -= 1;
                }
                break;
            case DOWN:
                if (menuSelect < 7) {
                    GotoXY(6, mY);
                    printf("    ");
                    mY += 2;
                    GotoXY(6, mY);
                    printf("▶");
                    menuSelect += 1;
                }
                break;
            }
        }
    }
    return menuSelect;
}

void AddBook(JSON_Array* array, JSON_Value* rootValue) {
    char title[100];
    char author[100];

    printf("추가할 도서 제목을 입력하세요: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0'; // 개행 문자 제거

    printf("추가할 도서 작가명을 입력하세요: ");
    fgets(author, sizeof(author), stdin);
    author[strcspn(author, "\n")] = '\0'; // 개행 문자 제거

    // 새 객체 생성
    JSON_Value* newBookValue = json_value_init_object();
    JSON_Object* newBook = json_value_get_object(newBookValue);

    // 새 객체에 값 추가
    json_object_set_string(newBook, "Title", title);
    json_object_set_string(newBook, "Author", author);

    // 배열에 새 객체 추가
    json_array_append_value(array, newBookValue);

    // 수정된 JSON을 파일에 저장
    json_serialize_to_file_pretty(rootValue, "Book.json");
    printf("도서가 추가되었습니다!\n");
}

void UpdateBook(JSON_Array* array, JSON_Value* rootValue, size_t size) {
    size_t array_size = json_array_get_count(array);
    if (array_size == 0) {
        printf("수정할 도서가 없습니다.\n");
        return;
    }
    size_t index = size / 2;
    // 선택된 도서 가져오기
    JSON_Object* obj = json_array_get_object(array, index);
    if (obj == NULL) {
        printf("도서를 가져오는 데 실패했습니다.\n");
        return;
    }

    // 수정할 키 선택
    printf("제목을 수정하고 싶으면 1, 작가명을 수정하고 싶으면 2,메뉴로 돌아가고 싶으면 3을 눌러주세요 :\n");
    int choice;
    scanf("%d", &choice);
    getchar(); // 입력 버퍼 비우기

    char newValue[100]; // 수정할 새 값
    switch (choice) {
    case 1:
        printf("새로운 제목을 입력하세요: ");
        fgets(newValue, sizeof(newValue), stdin);
        newValue[strcspn(newValue, "\n")] = '\0'; // 개행 문자 제거
        json_object_set_string(obj, "Title", newValue); //수정
        break;
    case 2:
        printf("새로운 작가명을 입력하세요: ");
        fgets(newValue, sizeof(newValue), stdin);
        newValue[strcspn(newValue, "\n")] = '\0'; // 개행 문자 제거
        json_object_set_string(obj, "Author", newValue); //수정
        break;
    case 3:
        printf("수정이 취소되었습니다.\n");
        return;
    default:
        printf("잘못된 선택입니다.\n");
        return;
    }

    // 수정된 JSON을 파일에 저장
    json_serialize_to_file_pretty(rootValue, "Book.json");
    printf("도서 정보가 수정되었습니다!\n");
}

void RecommendBook(JSON_Array* array, JSON_Value* rootValue) {
    size_t array_size = json_array_get_count(array);
    if (array_size == 0) {
        printf("도서 목록이 비어있습니다.\n");
        return 1;
    }
    printf("==================== 추천 도서 목록====================\n");
    for (size_t i = 0; i < array_size; i++) {
        JSON_Object* obj = json_array_get_object(array, i);
        if (obj != NULL) {
            const char* title = json_object_get_string(obj, "Title"); //각 키의 값들을 불러옴
            const char* author = json_object_get_string(obj, "Author");
            double cnt = json_object_get_number(obj, "Count");
            if (cnt > 5) {

                printf("    Title: %s, Author: %s\n", title, author);
                printf("-------------------------------------------------------\n");
            }

        }
    }
}

void Rent(JSON_Array* array, JSON_Value* rootValue) {
    size_t array_size = json_array_get_count(array);
    if (array_size == 0) {
        printf("대출/반납 가능한 도서가 없습니다.\n");
        return 1;
    }
    int getC = 1, mY = 2;
    int want = 0;
    int size = 0;
    printf("도서 대출을 원하시면 1, 반납을 원하시면 2를 입력해주세요 :");
    scanf("%d", &want);
    getchar();
    if (want == 2) { //반납
        int able = 0;
        printf("=======================도서 목록============================\n");
        for (size_t i = 0; i < array_size; i++) {
            JSON_Object* obj = json_array_get_object(array, i);
            if (obj != NULL) {
                const char* title = json_object_get_string(obj, "Title");
                const char* author = json_object_get_string(obj, "Author");
                double rent = json_object_get_number(obj, "Rent");
                if (rent == 0.0f) {
                    printf("    Title: %s, Author: %s, 대출 가능\n", title, author);
                    printf("-------------------------------------------------------\n");
                }
                else if (rent == 1.0f) {
                    printf("    Title: %s, Author: %s, 대출 불가능\n", title, author);
                    printf("-------------------------------------------------------\n");
                }

            }
        }
        GotoXY(1, mY);
        printf("▶");
        while (getC != ENTER) {
            if (_kbhit()) {
                getC = _getch();
                switch (getC) {
                case UP:
                    if (size > 0) {
                        GotoXY(1, mY);
                        printf("  ");
                        mY -= 2;
                        GotoXY(1, mY);
                        printf("▶");
                        size -= 2;
                    }
                    break;
                case DOWN:
                    if (size < array_size) {
                        GotoXY(1, mY);
                        printf("  ");
                        mY += 2;
                        GotoXY(1, mY);
                        printf("▶");
                        size += 2;
                    }
                    break;
                case ENTER: //반납
                    system("cls");
                    JSON_Object* obj = json_array_get_object(array, size / 2);
                    double rent = json_object_get_number(obj, "Rent");
                    if (rent == 1.0f) { 
                        rent = 0.0f;
                        json_object_set_number(obj, "Rent", rent); //값 변경
                        printf("반납되었습니다! 감사합니다.\n");
                        array = json_value_get_array(rootValue);
                        json_serialize_to_file_pretty(rootValue, "Book.json"); //파일에 저장
                        break;
                    }
                    else {
                        printf("대출중인 도서가 아닙니다.\n");
                        break;
                    }

                }
            }
        }
    }
    else if (want == 1 ) { //대출
        size_t array_size = json_array_get_count(array);
        if (array_size == 0) {
            printf("대출/반납 가능한 도서가 없습니다.\n");
            return 1;
        }

        printf("=======================도서 목록============================\n");
        for (size_t i = 0; i < array_size; i++) {
            JSON_Object* obj = json_array_get_object(array, i);
            if (obj != NULL) {
                const char* title = json_object_get_string(obj, "Title");
                const char* author = json_object_get_string(obj, "Author");
                double rent = json_object_get_number(obj, "Rent");
                if (rent == 0.0f) {
                    printf("    Title: %s, Author: %s, 대출 가능\n", title, author);
                    printf("-------------------------------------------------------\n");
                }
                else if (rent == 1.0f) {
                    printf("    Title: %s, Author: %s, 대출 불가능\n", title, author);
                    printf("-------------------------------------------------------\n");
                }

            }
        }

        GotoXY(1, mY);
        printf("▶");
        GotoXY(1, mY);
        printf("▶");
        while (getC != ENTER) {
            if (_kbhit()) {
                getC = _getch();
                switch (getC) {
                case UP:
                    if (size > 0) {
                        GotoXY(1, mY);
                        printf("  ");
                        mY -= 2;
                        GotoXY(1, mY);
                        printf("▶");
                        size -= 2;
                    }
                    break;
                case DOWN:
                    if (size < array_size) {
                        GotoXY(1, mY);
                        printf("  ");
                        mY += 2;
                        GotoXY(1, mY);
                        printf("▶");
                        size += 2;
                    }
                    break;
                case ENTER: //대출
                    system("cls");
                    JSON_Object* obj = json_array_get_object(array, size / 2);
                    double rent = json_object_get_number(obj, "Rent");
                    if (rent == 1.0f) {
                        printf("이미 대출중인 도서입니다.\n");
                        break;
                    }
                    else {
                        rent = 1.0f;
                        json_object_set_number(obj, "Rent", rent);
                        printf("대출되었습니다!\n");
                        array = json_value_get_array(rootValue);
                        json_serialize_to_file_pretty(rootValue, "Book.json");
                        break;
                    }

                }
            }
        }
    }
    else {
        printf("유효하지 않은 문자입니다.\n");
    }

}

void SearchBooks(JSON_Array* array, JSON_Value* rootValue) {
    printf("    1. 작가명으로 찾기 \n");
    printf("    2. 도서명으로 찾기 \n");
    printf("    3. 메뉴로 나가기 \n");

    int getC = 1, mY = 0;
    int select = 1;
    GotoXY(1, mY);
    printf("▶");
    while (getC != ENTER) {
        if (_kbhit()) {
            getC = _getch();
            switch (getC) {
            case UP:
                if (select > 1) {
                    GotoXY(1, mY);
                    printf("  ");
                    mY -= 1;
                    GotoXY(1, mY);
                    printf("▶");
                    select -= 1;
                }
                break;
            case DOWN:
                if (select < 3) {
                    GotoXY(1, mY);
                    printf("  ");
                    mY += 1;
                    GotoXY(1, mY);
                    printf("▶");
                    select += 1;
                }
                break;
            case ENTER:
                switch (select) {
                case 1:
                    SearchAuthor(array, rootValue); //작가명으로 찾기
                    break;
                case 2:
                    SearchBookname(array, rootValue); //도서명으로 찾기
                    break;
                }
            }
        }
    }
}

void SearchAuthor(JSON_Array* array, JSON_Value* rootValue) {
    system("cls");
    size_t array_size = json_array_get_count(array);
    if (array_size == 0) {
        printf("Failed to get JSON array.\n");
        return 1;
    }
    // 작가명으로 검색할 수 있는 기능 추가
    char bookAuthor[100];
    int found = 0;
    printf("작가명을 입력해주세요 : ");
    fgets(bookAuthor, sizeof(bookAuthor), stdin);
    bookAuthor[strcspn(bookAuthor, "\n")] = '\0';

    for (size_t i = 0; i < array_size; i++) {
        JSON_Object* obj = json_array_get_object(array, i);
        if (obj != NULL) {
            const char* title = json_object_get_string(obj, "Title");
            const char* author = json_object_get_string(obj, "Author");
            double cnt = json_object_get_number(obj, "Count");
            if (strcmp(author, bookAuthor) == 0) { //이름과 동일할때
                printf("----------------------------------------------------------\n");
                printf("    Title: %s, Author: %s\n", title, author);
                printf("----------------------------------------------------------\n");
                cnt = cnt + 1.0f; //검색횟수 증가
                json_object_set_number(obj, "Count", cnt);
                found = 1;
                break;
            }

        }
    }


    if (!found) {
        printf("찾으시는 작가님이 없습니다.\n");
    }
    json_serialize_to_file_pretty(rootValue, "Book.json"); //파일저장
}

void SearchBookname(JSON_Array* array, JSON_Value* rootValue) {
    system("cls");
    size_t array_size = json_array_get_count(array);
    if (array_size == 0) {
        printf("도서 목록이 비어있습니다.\n");
        return 1;
    }
    char bookName[100];
    int able = 0;
    printf("도서명을 입력해주세요 : ");
    fgets(bookName, sizeof(bookName), stdin);
    bookName[strcspn(bookName, "\n")] = '\0';

    for (size_t i = 0; i < array_size; i++) {
        JSON_Object* obj = json_array_get_object(array, i);
        if (obj != NULL) {
            const char* title = json_object_get_string(obj, "Title");
            const char* author = json_object_get_string(obj, "Author");
            double cnt = json_object_get_number(obj, "Count");
            if (strcmp(title, bookName) == 0) {
                printf("----------------------------------------------------------\n");
                printf("    Title: %s, Author: %s\n", title, author);
                printf("----------------------------------------------------------\n");
                cnt = cnt + 1.0f;
                printf("%lf", cnt);
                json_object_set_number(obj, "Count", cnt);
                able = 1;
            }

        }
    }
    if (!able) {
        printf("찾으시는 도서가 없습니다.\n");
    }
    json_serialize_to_file_pretty(rootValue, "Book.json");
}

void Menu() {
    printf("********************************************\n");
    printf("              도서 검색 프로그램           \n");
    printf("********************************************\n");
    printf("           1 : 전체 도서 목록              \n");
    printf("--------------------------------------------\n");
    printf("           2 : 추천 도서 목록              \n");
    printf("--------------------------------------------\n");
    printf("           3 : 도서 추가                   \n");
    printf("--------------------------------------------\n");
    printf("           4 : 도서 검색                   \n");
    printf("--------------------------------------------\n");
    printf("           5 : 도서 대출/반납              \n");
    printf("--------------------------------------------\n");
    printf("           6 : 초기화                   \n");
    printf("--------------------------------------------\n");
    printf("           7 : 종료 및 저장                   \n");
    printf("********************************************\n");
}