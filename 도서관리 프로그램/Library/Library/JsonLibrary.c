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

void GotoXY(int x, int y) { //Ŀ���̵��Լ�
    COORD Cur = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void HideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo); // ���� Ŀ�� ������ ������
    cursorInfo.bVisible = FALSE;                // Ŀ���� ����
    SetConsoleCursorInfo(hConsole, &cursorInfo); // ����� Ŀ�� ���� ����
}

void Menu(); //��ü �޴�
int SelectMenu(); //�޴� �����ϱ�
void AddBook(JSON_Array* array, JSON_Value* rootValue); //���� �߰�
void SearchBooks(JSON_Array* array, JSON_Value* rootValue); //���� �˻�
void SearchAuthor(JSON_Array* array, JSON_Value* rootValue); //�۰������� �˻�
void SearchBookname(JSON_Array* array, JSON_Value* rootValue); //���������� �˻�
void Rent(JSON_Array* array, JSON_Value* rootValue); //����/�ݳ�
void PrintBook(JSON_Array* array, JSON_Value* rootValue); // ���Ͽ��� å �о����
void RecommendBook(JSON_Array* array, JSON_Value* rootValue); //��õ ���� ���
void UpdateBook(JSON_Array* array, JSON_Value* rootValue, size_t size); //����

int main() {
    JSON_Value* rootValue = json_parse_file("Book.json"); //������ �о �Ľ�
    JSON_Object* rootObject = json_value_get_object(rootValue); // object������ ���
    if (rootValue == NULL) { // ������ ������
        printf("���� ����� ����ֽ��ϴ�.\n");
        return 1;
    }
    JSON_Array* array = json_value_get_array(rootValue); //�Ľ��Ѱ� �迭�� ����
    if (array == NULL) { // �迭�� ������ �޸� ���������ֱ�
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
            //�ʱ�ȭ
            rootValue = json_value_init_object();             // JSON_Value ���� �� �ʱ�ȭ
            rootObject = json_value_get_object(rootValue);    // JSON_Value���� JSON_Object�� ����
            json_serialize_to_file_pretty(rootValue, "Book.json"); //����� �б� ���� ���ڿ��� ����� ���� ����
            json_value_free(rootValue); // �޸� ����
            array = json_value_get_array(rootValue); // �ʱ�ȭ�� �迭 ���
            printf("�ʱ�ȭ �Ϸ�\n");
            break;
        case 7:
            return 0;
        }
        printf("\n ���͸� ���� �޴��� ���ư��ϴ�");
        getchar();
        system("cls");
    }
    json_value_free(rootValue);
    return 0;
}

void PrintBook(JSON_Array* array, JSON_Value* rootValue) {
    size_t array_size = json_array_get_count(array); // �迭 ũ�� ���
    if (array_size == 0) { // �迭 ũ�Ⱑ 0�̸� ����
        printf("Failed to get JSON array.\n");
        return 1;
    }
    printf("========================���� ���=========================\n");
    for (size_t i = 0; i < array_size; i++) {
        JSON_Object* obj = json_array_get_object(array, i); // i��° �迭�� ����
        if (obj != NULL) {
            const char* title = json_object_get_string(obj, "Title"); //Ű�� Title�ΰ��� ���� ������
            const char* author = json_object_get_string(obj, "Author"); //Ű�� Author �ΰ��� ���� ������
            printf("    Title: %s, Author: %s\n", title, author);
            printf("----------------------------------------------------------\n");
        }
    }
    int getC = 1, mY = 1;
    int edit = 0;
    size_t size = 0;
    GotoXY(1, mY);
    printf("��");
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
                    printf("��");
                    size -= 2;
                }
                break;
            case DOWN:
                if (size < array_size) {
                    GotoXY(1, mY);
                    printf("  ");
                    mY += 2;
                    GotoXY(1, mY);
                    printf("��");
                    size += 2;
                }
                break;
            case ENTER:
                system("cls");
                printf("������ ���Ͻø� 1, ������ ���Ͻø� 2, �޴��� ���ư��⸦ ���Ͻø� 3�� �Է����ּ��� :");
                scanf("%d", &edit);
                getchar();

                system("cls");
                if (edit == 1) { //����
                    system("cls");
                    UpdateBook(array, rootValue, size);
                }
                else if (edit == 2) { //����
                    printf("������ �����մϴ�.\n");
                    json_array_remove(array, size / 2); //�ε����� �����Ͽ� ����
                    json_serialize_to_file_pretty(rootValue, "Book.json"); //�����Ѱ� ����
                    printf("������ �����Ǿ����ϴ�!\n");
                }
                break;
            }
        }
    }
}

int SelectMenu() {
    Menu();
    int menuSelect = 1; //�޴� ����
    int getC = 1, mY = 3;

    GotoXY(6, mY);
    printf("��");
    while (getC != ENTER) {
        if (_kbhit()) { //Ű���尡 �Էµ� �����ΰ�?
            getC = _getch(); //������ �ƽ�Ű�ڵ�� ����
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

void AddBook(JSON_Array* array, JSON_Value* rootValue) {
    char title[100];
    char author[100];

    printf("�߰��� ���� ������ �Է��ϼ���: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0'; // ���� ���� ����

    printf("�߰��� ���� �۰����� �Է��ϼ���: ");
    fgets(author, sizeof(author), stdin);
    author[strcspn(author, "\n")] = '\0'; // ���� ���� ����

    // �� ��ü ����
    JSON_Value* newBookValue = json_value_init_object();
    JSON_Object* newBook = json_value_get_object(newBookValue);

    // �� ��ü�� �� �߰�
    json_object_set_string(newBook, "Title", title);
    json_object_set_string(newBook, "Author", author);

    // �迭�� �� ��ü �߰�
    json_array_append_value(array, newBookValue);

    // ������ JSON�� ���Ͽ� ����
    json_serialize_to_file_pretty(rootValue, "Book.json");
    printf("������ �߰��Ǿ����ϴ�!\n");
}

void UpdateBook(JSON_Array* array, JSON_Value* rootValue, size_t size) {
    size_t array_size = json_array_get_count(array);
    if (array_size == 0) {
        printf("������ ������ �����ϴ�.\n");
        return;
    }
    size_t index = size / 2;
    // ���õ� ���� ��������
    JSON_Object* obj = json_array_get_object(array, index);
    if (obj == NULL) {
        printf("������ �������� �� �����߽��ϴ�.\n");
        return;
    }

    // ������ Ű ����
    printf("������ �����ϰ� ������ 1, �۰����� �����ϰ� ������ 2,�޴��� ���ư��� ������ 3�� �����ּ��� :\n");
    int choice;
    scanf("%d", &choice);
    getchar(); // �Է� ���� ����

    char newValue[100]; // ������ �� ��
    switch (choice) {
    case 1:
        printf("���ο� ������ �Է��ϼ���: ");
        fgets(newValue, sizeof(newValue), stdin);
        newValue[strcspn(newValue, "\n")] = '\0'; // ���� ���� ����
        json_object_set_string(obj, "Title", newValue); //����
        break;
    case 2:
        printf("���ο� �۰����� �Է��ϼ���: ");
        fgets(newValue, sizeof(newValue), stdin);
        newValue[strcspn(newValue, "\n")] = '\0'; // ���� ���� ����
        json_object_set_string(obj, "Author", newValue); //����
        break;
    case 3:
        printf("������ ��ҵǾ����ϴ�.\n");
        return;
    default:
        printf("�߸��� �����Դϴ�.\n");
        return;
    }

    // ������ JSON�� ���Ͽ� ����
    json_serialize_to_file_pretty(rootValue, "Book.json");
    printf("���� ������ �����Ǿ����ϴ�!\n");
}

void RecommendBook(JSON_Array* array, JSON_Value* rootValue) {
    size_t array_size = json_array_get_count(array);
    if (array_size == 0) {
        printf("���� ����� ����ֽ��ϴ�.\n");
        return 1;
    }
    printf("==================== ��õ ���� ���====================\n");
    for (size_t i = 0; i < array_size; i++) {
        JSON_Object* obj = json_array_get_object(array, i);
        if (obj != NULL) {
            const char* title = json_object_get_string(obj, "Title"); //�� Ű�� ������ �ҷ���
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
        printf("����/�ݳ� ������ ������ �����ϴ�.\n");
        return 1;
    }
    int getC = 1, mY = 2;
    int want = 0;
    int size = 0;
    printf("���� ������ ���Ͻø� 1, �ݳ��� ���Ͻø� 2�� �Է����ּ��� :");
    scanf("%d", &want);
    getchar();
    if (want == 2) { //�ݳ�
        int able = 0;
        printf("=======================���� ���============================\n");
        for (size_t i = 0; i < array_size; i++) {
            JSON_Object* obj = json_array_get_object(array, i);
            if (obj != NULL) {
                const char* title = json_object_get_string(obj, "Title");
                const char* author = json_object_get_string(obj, "Author");
                double rent = json_object_get_number(obj, "Rent");
                if (rent == 0.0f) {
                    printf("    Title: %s, Author: %s, ���� ����\n", title, author);
                    printf("-------------------------------------------------------\n");
                }
                else if (rent == 1.0f) {
                    printf("    Title: %s, Author: %s, ���� �Ұ���\n", title, author);
                    printf("-------------------------------------------------------\n");
                }

            }
        }
        GotoXY(1, mY);
        printf("��");
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
                        printf("��");
                        size -= 2;
                    }
                    break;
                case DOWN:
                    if (size < array_size) {
                        GotoXY(1, mY);
                        printf("  ");
                        mY += 2;
                        GotoXY(1, mY);
                        printf("��");
                        size += 2;
                    }
                    break;
                case ENTER: //�ݳ�
                    system("cls");
                    JSON_Object* obj = json_array_get_object(array, size / 2);
                    double rent = json_object_get_number(obj, "Rent");
                    if (rent == 1.0f) { 
                        rent = 0.0f;
                        json_object_set_number(obj, "Rent", rent); //�� ����
                        printf("�ݳ��Ǿ����ϴ�! �����մϴ�.\n");
                        array = json_value_get_array(rootValue);
                        json_serialize_to_file_pretty(rootValue, "Book.json"); //���Ͽ� ����
                        break;
                    }
                    else {
                        printf("�������� ������ �ƴմϴ�.\n");
                        break;
                    }

                }
            }
        }
    }
    else if (want == 1 ) { //����
        size_t array_size = json_array_get_count(array);
        if (array_size == 0) {
            printf("����/�ݳ� ������ ������ �����ϴ�.\n");
            return 1;
        }

        printf("=======================���� ���============================\n");
        for (size_t i = 0; i < array_size; i++) {
            JSON_Object* obj = json_array_get_object(array, i);
            if (obj != NULL) {
                const char* title = json_object_get_string(obj, "Title");
                const char* author = json_object_get_string(obj, "Author");
                double rent = json_object_get_number(obj, "Rent");
                if (rent == 0.0f) {
                    printf("    Title: %s, Author: %s, ���� ����\n", title, author);
                    printf("-------------------------------------------------------\n");
                }
                else if (rent == 1.0f) {
                    printf("    Title: %s, Author: %s, ���� �Ұ���\n", title, author);
                    printf("-------------------------------------------------------\n");
                }

            }
        }

        GotoXY(1, mY);
        printf("��");
        GotoXY(1, mY);
        printf("��");
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
                        printf("��");
                        size -= 2;
                    }
                    break;
                case DOWN:
                    if (size < array_size) {
                        GotoXY(1, mY);
                        printf("  ");
                        mY += 2;
                        GotoXY(1, mY);
                        printf("��");
                        size += 2;
                    }
                    break;
                case ENTER: //����
                    system("cls");
                    JSON_Object* obj = json_array_get_object(array, size / 2);
                    double rent = json_object_get_number(obj, "Rent");
                    if (rent == 1.0f) {
                        printf("�̹� �������� �����Դϴ�.\n");
                        break;
                    }
                    else {
                        rent = 1.0f;
                        json_object_set_number(obj, "Rent", rent);
                        printf("����Ǿ����ϴ�!\n");
                        array = json_value_get_array(rootValue);
                        json_serialize_to_file_pretty(rootValue, "Book.json");
                        break;
                    }

                }
            }
        }
    }
    else {
        printf("��ȿ���� ���� �����Դϴ�.\n");
    }

}

void SearchBooks(JSON_Array* array, JSON_Value* rootValue) {
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
                    SearchAuthor(array, rootValue); //�۰������� ã��
                    break;
                case 2:
                    SearchBookname(array, rootValue); //���������� ã��
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
    // �۰������� �˻��� �� �ִ� ��� �߰�
    char bookAuthor[100];
    int found = 0;
    printf("�۰����� �Է����ּ��� : ");
    fgets(bookAuthor, sizeof(bookAuthor), stdin);
    bookAuthor[strcspn(bookAuthor, "\n")] = '\0';

    for (size_t i = 0; i < array_size; i++) {
        JSON_Object* obj = json_array_get_object(array, i);
        if (obj != NULL) {
            const char* title = json_object_get_string(obj, "Title");
            const char* author = json_object_get_string(obj, "Author");
            double cnt = json_object_get_number(obj, "Count");
            if (strcmp(author, bookAuthor) == 0) { //�̸��� �����Ҷ�
                printf("----------------------------------------------------------\n");
                printf("    Title: %s, Author: %s\n", title, author);
                printf("----------------------------------------------------------\n");
                cnt = cnt + 1.0f; //�˻�Ƚ�� ����
                json_object_set_number(obj, "Count", cnt);
                found = 1;
                break;
            }

        }
    }


    if (!found) {
        printf("ã���ô� �۰����� �����ϴ�.\n");
    }
    json_serialize_to_file_pretty(rootValue, "Book.json"); //��������
}

void SearchBookname(JSON_Array* array, JSON_Value* rootValue) {
    system("cls");
    size_t array_size = json_array_get_count(array);
    if (array_size == 0) {
        printf("���� ����� ����ֽ��ϴ�.\n");
        return 1;
    }
    char bookName[100];
    int able = 0;
    printf("�������� �Է����ּ��� : ");
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
        printf("ã���ô� ������ �����ϴ�.\n");
    }
    json_serialize_to_file_pretty(rootValue, "Book.json");
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