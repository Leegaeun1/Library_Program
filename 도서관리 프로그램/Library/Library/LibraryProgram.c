//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <windows.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#define UP 72
//#define DOWN 80
//#define ENTER 13
//
//void GotoXY(int x, int y) { //Ŀ���̵��Լ�
//    COORD Cur = { x,y };
//    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
//}
//
//void HideCursor() {
//    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//    CONSOLE_CURSOR_INFO cursorInfo;
//
//    GetConsoleCursorInfo(hConsole, &cursorInfo); // ���� Ŀ�� ������ ������
//    cursorInfo.bVisible = FALSE;                // Ŀ���� ����
//    SetConsoleCursorInfo(hConsole, &cursorInfo); // ����� Ŀ�� ���� ����
//}
//
//typedef struct Book {
//    char name[100];
//    char author[100];
//    int searchCnt;
//    int rent;
//    struct Book* prev;
//    struct Book* next;
//} Book;
//static int cnt = 0;
//Book* head = NULL,* tail = NULL;
//Book* book = NULL;
//
//void Menu(); //��ü �޴�
//int SelectMenu(); //�޴� �����ϱ�
//void AddBook(); //���� �߰�
//void PrintBooks(); //���� ���
//void SaveFile(FILE* fp); //����
//void FreeBooks(); //�޸� �Ҵ� ����
//void SearchBooks(); //���� �˻�
//void SearchAuthor(); //�۰������� �˻�
//void SearchBookname(); //���������� �˻�
//void ResetFile(FILE* fp); //�ʱ�ȭ
//void Rent(); //����/�ݳ�
//void LoadBook(FILE* fp); // ���Ͽ��� å �о����
//void RecommendBook(); //��õ ���� ���
//
//int main() {
//    FILE* fp = fopen("books.txt", "rb");
//    if (fp == NULL) {
//        fprintf(stderr, "books.txt ������ ã�� �� �����ϴ�.\n");
//        return;
//    }
//    LoadBook(fp);
//    int selectedMenu;
//    HideCursor();
//    while (1) {
//        selectedMenu = SelectMenu();
//        system("cls");
//        switch (selectedMenu) {
//        case 1:
//            PrintBooks();
//            break;
//        case 2:
//            RecommendBook();
//            break;
//        case 3:
//            AddBook();
//            break;
//        case 4:
//            SearchBooks();
//            break;
//        case 5:
//            Rent();
//            break;
//        case 6:
//            fp = fopen("books.txt", "w");
//            if (fp == NULL) {
//                fprintf(stderr, "books.txt ������ ã�� �� �����ϴ�.\n");
//                return;
//            }
//            ResetFile(fp);
//            printf("�ʱ�ȭ �Ϸ�\n");
//            break;
//        case 7:
//            fp = fopen("books.txt", "wb");
//            if (fp == NULL) {
//                fprintf(stderr, "books������ ã�� �� �����ϴ�.", "books.txt");
//                exit(1);
//            }
//            SaveFile(fp);
//            fclose(fp);
//            return 0;
//        }
//        printf("\n ���͸� ���� �޴��� ���ư��ϴ�");
//        getchar();
//        system("cls");
//    }
//    FreeBooks();
//
//
//    return 0;
//}
//
//void LoadBook(FILE* fp) {
//    if (fp == NULL) { // ������ ���� ��� �ʱ�ȭ
//        printf("������ �������� �ʽ��ϴ�. ���ο� �����͸� �߰��ϼ���.\n");
//        return;
//    }
//
//    int read = 0;
//    while (1) {
//        Book* book = (Book*)malloc(sizeof(Book)); // �� ���� ����
//        if (book == NULL) {
//            fprintf(stderr, "�޸� �Ҵ� ����\n");
//            exit(1);
//        }
//        read = fread(book, sizeof(Book), 1, fp); // �б� �õ�
//        if (read != 1) { // �� �̻� ���� �����Ͱ� ������ ����
//            free(book);
//            break;
//        }
//
//        if (head == NULL) { // ���� ����Ʈ�� ����ִٸ� ù ���� ����
//            head = book;
//            book->prev = NULL;
//        }
//        else { // ���� ���� ����Ʈ�� �߰�
//            tail->next = book;
//            book->prev = tail;
//        }
//        book->next = NULL;
//        tail = book;
//        cnt++;
//    }
//    fclose(fp); // ���� �ݱ�
//}
//
//
//int SelectMenu() {
//    Menu();
//    int menuSelect = 1; //�޴� ����
//    int getC = 1, mY = 3;
//
//    GotoXY(6, mY);
//    printf("��");
//    while (getC != ENTER) {
//        if (_kbhit()) { //Ű���尡 �Էµ� �����ΰ�?
//            getC = _getch(); //������ �ƽ�Ű�ڵ�� ����
//            switch (getC) {
//            case UP:
//                if (menuSelect > 1) {
//                    GotoXY(6, mY);
//                    printf("    ");
//                    mY -= 2;
//                    GotoXY(6, mY);
//                    printf("��");
//                    menuSelect -= 1;
//                }
//                break;
//            case DOWN:
//                if (menuSelect < 7) {
//                    GotoXY(6, mY);
//                    printf("    ");
//                    mY += 2;
//                    GotoXY(6, mY);
//                    printf("��");
//                    menuSelect += 1;
//                }
//                break;
//            }
//        }
//    }
//    return menuSelect;
//}
//
//void AddBook() {
//    int cnt = 0;
//    book = (Book*)malloc(sizeof(Book));
//    printf("�߰��� �������� �Է����ּ��� : ");
//    scanf_s("%[^\n]s", book->name, sizeof(book->name));
//    getchar();
//
//    printf("�۰����� �Է����ּ��� : ");
//    scanf_s("%[^\n]s", book->author, sizeof(book->author));
//    getchar();
//
//    book->searchCnt = 0; //�ʱ� �˻� Ƚ��
//    book->rent = 0;
//
//    if (head == NULL) {
//        head = tail = book;
//        book->prev = NULL;
//        book->next = NULL;
//    }
//    else {
//        tail->next = book;
//        book->prev = tail;
//        tail = book;
//        tail->next = NULL;
//    }
//    printf("������ ���������� �߰��Ǿ����ϴ�.\n");
//    while (cnt != ENTER) {
//        if (_kbhit()) {
//            cnt = _getch();
//        }
//    }
//}
//
//void PrintBooks() {
//    Book* tmp = head;
//    if (tmp == NULL) {
//        printf("���� ����� ����ֽ��ϴ�.\n");
//        return;
//    }
//
//    printf("========================���� ���=========================\n");
//    while (tmp != NULL) {
//        printf("             ������: %s, �۰���: %s\n", tmp->name, tmp->author);
//        printf("----------------------------------------------------------\n");
//        tmp = tmp->next;
//    }
//    int getC = 1, mY = 1;
//    int edit = 0;
//    tmp = head;
//    GotoXY(1, mY);
//    printf("��");
//    while (getC != ENTER) {
//        if (_kbhit()) {
//            getC = _getch();
//            switch (getC) {
//            case UP:
//                if (tmp->prev != NULL) {
//                    GotoXY(1, mY);
//                    printf("  ");
//                    mY -= 2;
//                    GotoXY(1, mY);
//                    printf("��");
//                    tmp = tmp->prev;
//                }
//                break;
//            case DOWN:
//                if (tmp->next != NULL) {
//                    GotoXY(1, mY);
//                    printf("  ");
//                    mY += 2;
//                    GotoXY(1, mY);
//                    printf("��");
//                    tmp = tmp->next;
//                }
//                break;
//            case ENTER: //����, ����
//                system("cls");
//                printf("������ ���Ͻø� 1, ������ ���Ͻø� 2, �޴��� ���ư��⸦ ���Ͻø� 3�� �Է����ּ��� :");
//                scanf("%d", &edit);
//                getchar();
//
//                system("cls");
//                if (edit == 1) {
//                    printf("�����Ͻð���� å ������ �Է����ּ��� :");
//                    scanf("%s", tmp->name);
//                    getchar();
//
//                    printf("�����Ͻð���� �۰����� �Է����ּ��� :");
//                    scanf("%s", tmp->author);
//                    getchar();
//
//                }
//                else if (edit == 2) {
//                    printf("������ �����մϴ�.\n");
//
//                    // ù ��° ������� Ȯ��
//                    if (tmp->prev == NULL) {
//                        head = tmp->next; // head�� ���� ���� ����
//                        if (head != NULL) {
//                            head->prev = NULL; // �� head�� prev�� NULL�� ����
//                        }
//                    }
//                    else {
//                        tmp->prev->next = tmp->next; // ���� ����� next�� ���� ����� next�� ����
//                    }
//
//                    if (tmp->next != NULL) {
//                        tmp->next->prev = tmp->prev; // ���� ����� prev�� ���� ����� prev�� ����
//                    }
//
//                    // ���� ����� �޸� ����
//                    free(tmp);
//
//                    printf("������ �����Ǿ����ϴ�!\n");
//
//                }
//                break;
//            }
//        }
//    }
//}
//
//void RecommendBook() {
//    Book* tmp = head;
//    if (tmp == NULL) {
//        printf("���� ����� ����ֽ��ϴ�.\n");
//        return;
//    }
//    printf("==================== ��õ ���� ���====================\n");
//    while (tmp != NULL) {
//        if (tmp->searchCnt > 5) {
//            printf("           ������: %s, �۰���: %s     \n", tmp->name, tmp->author);
//            printf("--------------------------------------------------\n");
//        }
//        tmp = tmp->next;
//    }
//}
//
//void SaveFile(FILE* fp) {
//    if (fp == NULL) {
//        printf("���� �����Ͱ� NULL�Դϴ�. ������ �� �� �����ϴ�.\n");
//        return;
//    }
//
//    book = head;
//    while (book != NULL) {
//        // fwrite�� ����Ͽ� å ������ ���Ͽ� ���̳ʸ� �������� ����
//        if (fwrite(book, sizeof(Book), 1, fp) != 1) {
//            printf("���Ͽ� ���� ���� �߻�!\n");
//            return;
//        }
//        book = book->next;  // ���� å���� �̵�
//    }
//    fclose(fp);  // ���� �ݱ�
//}
//
//void Rent() {
//    book = head;
//    //char bookName[100];
//    int isRent = 0;
//
//    if (book == NULL) {
//        printf("����/�ݳ� ������ ������ �����ϴ�.\n");
//    }
//    int getC = 1, mY = 2;
//    int want = 0;
//    char bookName[100];
//    printf("���� ������ ���Ͻø� 1, �ݳ��� ���Ͻø� 2�� �Է����ּ��� :");
//    scanf("%d", &want);
//    getchar();
//    if (want == 2) {
//        int able = 0;
//
//        Book* tmp = head;
//        if (tmp == NULL) {
//            printf("���� ����� ����ֽ��ϴ�.\n");
//            return;
//        }
//
//        printf("=======================���� ���============================\n");
//        while (tmp != NULL) {
//            if (tmp->rent == 0) {
//                printf("           ������: %s, �۰���: %s, ���� ����\n", tmp->name, tmp->author);
//                printf("------------------------------------------------------------\n");
//            }
//            else if (tmp->rent == 1) {
//                printf("           ������: %s, �۰���: %s, ���� �Ұ���\n", tmp->name, tmp->author);
//                printf("------------------------------------------------------------\n");
//            }
//
//            tmp = tmp->next;
//        }
//
//        book = head;
//        GotoXY(1, mY);
//        printf("��");
//        while (getC != ENTER) {
//            if (_kbhit()) {
//                getC = _getch();
//                switch (getC) {
//                case UP:
//                    if (book->prev != NULL) {
//                        GotoXY(1, mY);
//                        printf("  ");
//                        mY -= 2;
//                        GotoXY(1, mY);
//                        printf("��");
//                        book = book->prev;
//                    }
//                    break;
//                case DOWN:
//                    if (book->next != NULL) {
//                        GotoXY(1, mY);
//                        printf("  ");
//                        mY += 2;
//                        GotoXY(1, mY);
//                        printf("��");
//                        book = book->next;
//                    }
//                    break;
//                case ENTER: //�ݳ�
//                    system("cls");
//                    if (book->rent == 1) {
//                        book->rent = 0;
//                        printf("�ݳ��Ǿ����ϴ�! �����մϴ�.\n");
//                        break;
//                    }
//                    else {
//                        printf("�������� ������ �ƴմϴ�.\n");
//                        break;
//                    }
//                    
//                }
//            }
//        }
//    }
//    else if(want==1 || want==2){
//        char confirm;
//        Book* tmp = head;
//        if (tmp == NULL) {
//            printf("���� ����� ����ֽ��ϴ�.\n");
//            return;
//        }
//
//        printf("=======================���� ���============================\n");
//        while (tmp != NULL) {
//            if (tmp->rent == 0) {
//                printf("           ������: %s, �۰���: %s, ���� ����\n", tmp->name, tmp->author);
//                printf("------------------------------------------------------------\n");
//            }
//            else if (tmp->rent == 1) {
//                printf("           ������: %s, �۰���: %s, ���� �Ұ���\n", tmp->name, tmp->author);
//                printf("------------------------------------------------------------\n");
//            }
//            
//            tmp = tmp->next;
//        }
//
//        book = head;
//        GotoXY(1, mY);
//        printf("��");
//        while (getC != ENTER) {
//            if (_kbhit()) {
//                getC = _getch();
//                switch (getC) {
//                case UP:
//                    if (book->prev!=NULL) {
//                        GotoXY(1, mY);
//                        printf("  ");
//                        mY -= 2;
//                        GotoXY(1, mY);
//                        printf("��");
//                        book = book->prev;
//                    }
//                    break;
//                case DOWN:
//                    if (book->next!=NULL) {
//                        GotoXY(1, mY);
//                        printf("  ");
//                        mY += 2;
//                        GotoXY(1, mY);
//                        printf("��");
//                        book = book->next;
//                    }
//                    break;
//                case ENTER: //����
//                    system("cls");
//                    if (book->rent == 0) {
//                        book->rent = 1;
//                        printf("����Ǿ����ϴ�.\n");                        
//                    }
//                    else {
//                        printf("�̹� ����� �����Դϴ�.\n");
//                    }
//                    break;
//                }
//            }
//        }
//    }
//    else {
//        printf("��ȿ���� ���� �����Դϴ�.\n");
//    }
//    
//}
//
//void FreeBooks() { 
//    Book* current = head;
//    while (current != NULL) {
//        Book* next = current->next;
//        free(current);
//        current = next;
//    }
//}
//
//void SearchBooks() {
//    printf("    1. �۰������� ã�� \n");
//    printf("    2. ���������� ã�� \n");
//    printf("    3. �޴��� ������ \n");
//
//    int getC = 1, mY = 0;
//    int select = 1;
//
//    GotoXY(1, mY);
//    printf("��");
//    while (getC != ENTER) {
//        if (_kbhit()) {
//            getC = _getch();
//            switch (getC) {
//            case UP:
//                if (select > 1) {
//                    GotoXY(1, mY);
//                    printf("  ");
//                    mY -= 1;
//                    GotoXY(1, mY);
//                    printf("��");
//                    select -= 1;
//                }
//                break;
//            case DOWN:
//                if (select < 3) {
//                    GotoXY(1, mY);
//                    printf("  ");
//                    mY += 1;
//                    GotoXY(1, mY);
//                    printf("��");
//                    select += 1;
//                }
//                break;
//            case ENTER:
//                switch (select) {
//                case 1:
//                    SearchAuthor();
//                    break;
//                case 2:
//                    SearchBookname();
//                    break;
//                }
//            }
//        }
//    }
//}
//
//void SearchAuthor() {
//    system("cls");
//    // �۰������� �˻��� �� �ִ� ��� �߰�
//    book = head;
//    char bookAuthor[100];
//    int able = 0;
//    printf("�۰����� �Է����ּ��� : ");
//    scanf("%[^\n]s", bookAuthor);
//    getchar();
//
//    while (book != NULL) {
//        if (strcmp(book->author, bookAuthor) == 0) {
//            able = 1;
//            book->searchCnt += 1;
//            printf("������: % s, �۰��� : % s\n", book->name, book->author);
//            break;
//        }
//        book = book->next;
//    }
//    if (!able) {
//        printf("ã���ô� �۰����� �����ϴ�.\n");
//    }
//}
//
//void SearchBookname() {
//    system("cls");
//    // ���������� �˻��� �� �ִ� ��� �߰�
//    book = head;
//    char bookName[100];
//    int able = 0;
//    printf("�������� �Է����ּ��� : ");
//    scanf("%[^\n]s", bookName);
//    getchar();
//
//    while (book != NULL) {
//        if (strcmp(book->name, bookName) == 0) {
//            able = 1;
//            book->searchCnt += 1;
//            printf("������: % s, �۰��� : % s\n", book->name, book->author);
//            break;
//        }
//        book = book->next;
//    }
//    if (!able) {
//        printf("ã���ô� ������ �����ϴ�.\n");
//    }
//}
//
//void ResetFile(FILE* fp) {
//    if (fp == NULL) {
//        printf("���� ���� ����\n");
//        return;
//    }
//    
//    char confirm;
//    printf("��� �����͸� �ʱ�ȭ�Ͻðڽ��ϱ�? (Y/N): ");
//    scanf(" %c", &confirm);
//    if (confirm != 'Y' && confirm != 'y') {
//        printf("�ʱ�ȭ�� ����߽��ϴ�.\n");
//        return;
//    }
//
//    fclose(fp);
//    FreeBooks();
//    head = tail = NULL;
//    printf("���� ������ �����Ǿ����ϴ�.\n");
//}
//
//void Menu() {
//    printf("********************************************\n");
//    printf("              ���� �˻� ���α׷�           \n");
//    printf("********************************************\n");
//    printf("           1 : ��ü ���� ���              \n");
//    printf("--------------------------------------------\n");
//    printf("           2 : ��õ ���� ���              \n");
//    printf("--------------------------------------------\n");
//    printf("           3 : ���� �߰�                   \n");
//    printf("--------------------------------------------\n");
//    printf("           4 : ���� �˻�                   \n");
//    printf("--------------------------------------------\n");
//    printf("           5 : ���� ����/�ݳ�              \n");
//    printf("--------------------------------------------\n");
//    printf("           6 : �ʱ�ȭ                   \n");
//    printf("--------------------------------------------\n");
//    printf("           7 : ���� �� ����                   \n");
//    printf("********************************************\n");
//}
