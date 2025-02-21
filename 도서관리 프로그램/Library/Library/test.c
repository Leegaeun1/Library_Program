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
    int rent;  // boolean ��� int�� ���� (0: ���� �� ��, 1: ���� ��)
    struct Book* prev;
    struct Book* next;
} Book;

Book* head = NULL;
Book* tail = NULL;

FILE* fp;

void Menu(); //��ü �޴�
void GotoXY(int x, int y); //�̵�
int selectMenu(); //�޴� �����ϱ�
void HideCursor(); //Ŀ�� �����
void AddBook(); //���� �߰�
void PrintBooks(); //���� ���
void SaveFile(); //����
void FreeBooks(); //�޸� �Ҵ� ����
void SearchBooks(); //���� �˻�
void SearchAuthor();
void SearchBookname();
void ResetFile(); //�ʱ�ȭ
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
            printf("��ü ���� ��� ���.\n");
            break;
        case 2:
            printf("��õ ���� ��� ���.\n");
            break;
        case 3:
            AddBook();
            SaveFile();
            break;
        case 4:
            SearchBooks();
            break;
        case 5:
            printf("���� ����/�ݳ�\n");
            break;
        case 6:
            ResetFile();
            printf("�ʱ�ȭ");
            break;
        case 7:
            SaveFile();
            printf("����\n");
            return 0;
        default:
            printf("����");
            break;
        }
        printf("\n ���͸� ���� �޴��� ���ư��ϴ�");
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
    printf("��");
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
                    printf("��");
                    menuSelect -= 1;
                }
                break;
            case DOWN:
                if (menuSelect < 7) {
                    GotoXY(6, mY);
                    printf("    ");
                    mY += 2;
                    GotoXY(6, mY);
                    printf("��");
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
        printf("�޸� �Ҵ� ����\n");
        return;
    }
    printf("�߰��� �������� �Է����ּ��� : ");
    fgets(book->name, sizeof(book->name), stdin);
    book->name[strcspn(book->name, "\n")] = '\0'; // ���� ���� ����

    printf("�۰����� �Է����ּ��� : ");
    fgets(book->author, sizeof(book->author), stdin);
    book->author[strcspn(book->author, "\n")] = '\0'; // ���� ���� ����

    book->researchCnt = 0; // �ʱ� �˻� Ƚ��
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
    printf("������ ���������� �߰��Ǿ����ϴ�.\n");
}

void PrintBooks() {
    FILE* fp = fopen("books.txt", "r");
    if (fp == NULL) {
        printf("���� ���� ����\n");
        return;
    }
    char str[100];
    while (fgets(str, sizeof(str), fp)) {
        str[strcspn(str, "\n")] = '\0';  // ���๮�� ����

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
        author[k] = '\0';  // �۰��� ��

        printf("������: %s\n", bookTitle);
        printf("�۰���: %s\n", author);
        printf("-------------------------------\n");
    }
    fclose(fp);
}

void SearchBooks() {
    printf("    1. �۰������� ã�� \n");
    printf("    2. ���������� ã�� \n");
    printf("    3. �޴��� ������ \n");

    int getC = 1, mY = 0;
    int select = 1;

    GotoXY(1, mY);
    printf("��");
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
                    printf("��");
                    select -= 1;
                }
                break;
            case DOWN:
                if (select < 3) {
                    GotoXY(1, mY);
                    printf("  ");
                    mY += 1;
                    GotoXY(1, mY);
                    printf("��");
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
        printf("���� ���� ����\n");
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
            printf("�޸� �Ҵ� ����\n");
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

    printf("ã���÷��� �۰����� �Է����ּ��� : ");
    scanf_s("%[^\n]s", searchAuthor, sizeof(searchAuthor));

    int found = 0;

    Book* tmp = head;
    while (tmp != NULL) {
        if (strcmp(tmp->author, searchAuthor) == 0) {
            printf("������: %s, �۰���: %s\n", tmp->name, tmp->author);
            found = 1;
        }
        tmp = tmp->next;
    }

    if (!found) {
        printf("ã���ô� �۰����� �����ϴ�.\n");
    }
}

void SearchBookname() {
    // ���������� ã�� ��� ����
}

void ResetFile() {
    FILE* fp = fopen("books.txt", "w");
    if (fp == NULL) {
        printf("���� ���� ����\n");
        return;
    }

    fclose(fp);
    printf("���� ������ �����Ǿ����ϴ�.\n");
}

void SaveFile() {
    FILE* fp = fopen("books.txt", "w");
    if (fp == NULL) {
        printf("���� ���� ����\n");
        return;
    }

    Book* tmp = head;
    while (tmp != NULL) {
        fprintf(fp, "%s,%s\n", tmp->name, tmp->author);
        tmp = tmp->next;
    }

    fclose(fp);
    printf("�����Ͱ� ���Ͽ� ����Ǿ����ϴ�.\n");
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
    printf("              ���� �˻� ���α׷�           \n");
    printf("********************************************\n");
    printf("           1 : ��ü ���� ���              \n");
    printf("--------------------------------------------\n");
    printf("           2 : ��õ ���� ���              \n");
    printf("--------------------------------------------\n");
    printf("           3 : ���� �߰�                   \n");
    printf("--------------------------------------------\n");
    printf("           4 : ���� �˻�                   \n");
    printf("--------------------------------------------\n");
    printf("           5 : ���� ����/�ݳ�              \n");
    printf("--------------------------------------------\n");
    printf("           6 : �ʱ�ȭ                   \n");
    printf("--------------------------------------------\n");
    printf("           7 : ���� �� ����                   \n");
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
