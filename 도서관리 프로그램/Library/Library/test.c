#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

#define UP 72
#define DOWN 80
#define ENTER 13

typedef struct Book {
    char name[100];
    char author[100];
    int researchCnt;
    int rent;  // boolean 대신 int로 수정 (0: 대출 안 됨, 1: 대출 됨)
    struct Book* prev;
    struct Book* next;
} Book;

Book* head = NULL;
Book* tail = NULL;

FILE* fp;

void Menu(); //전체 메뉴
void GotoXY(int x, int y); //이동
int selectMenu(); //메뉴 선택하기
void HideCursor(); //커서 숨기기
void AddBook(); //도서 추가
void PrintBooks(); //도서 출력
void SaveFile(); //저장
void FreeBooks(); //메모리 할당 해제
void SearchBooks(); //도서 검색
void SearchAuthor();
void SearchBookname();
void ResetFile(); //초기화
void LoadBooksFromFile();


int main() {
    LoadBooksFromFile();
    int selectedMenu;
    HideCursor();
    while (1) {
        selectedMenu = selectMenu();
        system("cls");

        switch (selectedMenu) {
        case 1:
            PrintBooks();
            printf("전체 도서 목록 출력.\n");
            break;
        case 2:
            printf("추천 도서 목록 출력.\n");
            break;
        case 3:
            AddBook();
            SaveFile();
            break;
        case 4:
            SearchBooks();
            break;
        case 5:
            printf("도서 대출/반납\n");
            break;
        case 6:
            ResetFile();
            printf("초기화");
            break;
        case 7:
            SaveFile();
            printf("종료\n");
            return 0;
        default:
            printf("오류");
            break;
        }
        printf("\n 엔터를 눌러 메뉴로 돌아갑니다");
        getchar();
        system("cls");
    }
    FreeBooks();
    return 0;
}

int selectMenu() {
    Menu();
    int menuSelect = 1;
    int getC = 1, mY = 3;

    GotoXY(6, mY);
    printf("▶");
    while (getC != ENTER) {
        if (_kbhit()) {
            getC = _getch();
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

void AddBook() {
    Book* book = (Book*)malloc(sizeof(Book));
    if (book == NULL) {
        printf("메모리 할당 실패\n");
        return;
    }
    printf("추가할 도서명을 입력해주세요 : ");
    fgets(book->name, sizeof(book->name), stdin);
    book->name[strcspn(book->name, "\n")] = '\0'; // 개행 문자 제거

    printf("작가명을 입력해주세요 : ");
    fgets(book->author, sizeof(book->author), stdin);
    book->author[strcspn(book->author, "\n")] = '\0'; // 개행 문자 제거

    book->researchCnt = 0; // 초기 검색 횟수
    book->rent = 0;
    book->prev = NULL;
    book->next = NULL;

    if (head == NULL) {
        head = tail = book;
    }
    else {
        tail->next = book;
        book->prev = tail;
        tail = book;
    }
    printf("도서가 성공적으로 추가되었습니다.\n");
}

void PrintBooks() {
    FILE* fp = fopen("books.txt", "r");
    if (fp == NULL) {
        printf("파일 열기 실패\n");
        return;
    }
    char str[100];
    while (fgets(str, sizeof(str), fp)) {
        str[strcspn(str, "\n")] = '\0';  // 개행문자 제거

        int i = 0;
        char bookTitle[50], author[50];
        int j = 0, k = 0;
        int commaFound = 0;

        while (str[i] != '\0') {
            if (str[i] == ',' && !commaFound) {
                bookTitle[j] = '\0';
                commaFound = 1;
                i++;
                continue;
            }
            if (commaFound) {
                author[k++] = str[i];
            }
            else {
                bookTitle[j++] = str[i];
            }
            i++;
        }
        author[k] = '\0';  // 작가명 끝

        printf("도서명: %s\n", bookTitle);
        printf("작가명: %s\n", author);
        printf("-------------------------------\n");
    }
    fclose(fp);
}

void SearchBooks() {
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
                    SearchAuthor();
                    break;
                case 2:
                    SearchBookname();
                    break;
                }
            }
        }
    }
}

void LoadBooksFromFile() {
    FILE* fp = fopen("books.txt", "r");
    if (fp == NULL) {
        printf("파일 열기 실패\n");
        return;
    }

    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        char bookName[100], author[100];
        int i = 0, j = 0;
        int commaFound = 0;

        line[strcspn(line, "\n")] = '\0';

        while (line[i] != '\0') {
            if (line[i] == ',' && !commaFound) {
                bookName[j] = '\0';
                commaFound = 1;
                i++;
                continue;
            }
            if (commaFound) {
                author[j++] = line[i];
            }
            else {
                bookName[j++] = line[i];
            }
            i++;
        }
        author[j] = '\0';

        Book* newBook = (Book*)malloc(sizeof(Book));
        if (newBook == NULL) {
            printf("메모리 할당 실패\n");
            fclose(fp);
            return;
        }
        strcpy_s(newBook->name, sizeof(newBook->name), bookName);
        strcpy_s(newBook->author, sizeof(newBook->author), author);
        newBook->researchCnt = 0;
        newBook->rent = 0;
        newBook->prev = tail;
        newBook->next = NULL;

        if (head == NULL) {
            head = tail = newBook;
        }
        else {
            tail->next = newBook;
            tail = newBook;
        }
    }
    fclose(fp);
}

void SearchAuthor() {
    system("cls");
    char searchAuthor[100];

    printf("찾으시려는 작가명을 입력해주세요 : ");
    scanf_s("%[^\n]s", searchAuthor, sizeof(searchAuthor));

    int found = 0;

    Book* tmp = head;
    while (tmp != NULL) {
        if (strcmp(tmp->author, searchAuthor) == 0) {
            printf("도서명: %s, 작가명: %s\n", tmp->name, tmp->author);
            found = 1;
        }
        tmp = tmp->next;
    }

    if (!found) {
        printf("찾으시는 작가님이 없습니다.\n");
    }
}

void SearchBookname() {
    // 도서명으로 찾는 기능 구현
}

void ResetFile() {
    FILE* fp = fopen("books.txt", "w");
    if (fp == NULL) {
        printf("파일 열기 실패\n");
        return;
    }

    fclose(fp);
    printf("파일 내용이 삭제되었습니다.\n");
}

void SaveFile() {
    FILE* fp = fopen("books.txt", "w");
    if (fp == NULL) {
        printf("파일 열기 실패\n");
        return;
    }

    Book* tmp = head;
    while (tmp != NULL) {
        fprintf(fp, "%s,%s\n", tmp->name, tmp->author);
        tmp = tmp->next;
    }

    fclose(fp);
    printf("데이터가 파일에 저장되었습니다.\n");
}

void FreeBooks() {
    Book* current = head;
    while (current != NULL) {
        Book* next = current->next;
        free(current);
        current = next;
    }
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

void GotoXY(int x, int y) {
    COORD Cur = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void HideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
