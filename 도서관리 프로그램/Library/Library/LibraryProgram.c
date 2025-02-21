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
//void GotoXY(int x, int y) { //커서이동함수
//    COORD Cur = { x,y };
//    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
//}
//
//void HideCursor() {
//    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//    CONSOLE_CURSOR_INFO cursorInfo;
//
//    GetConsoleCursorInfo(hConsole, &cursorInfo); // 현재 커서 정보를 가져옴
//    cursorInfo.bVisible = FALSE;                // 커서를 숨김
//    SetConsoleCursorInfo(hConsole, &cursorInfo); // 변경된 커서 정보 설정
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
//void Menu(); //전체 메뉴
//int SelectMenu(); //메뉴 선택하기
//void AddBook(); //도서 추가
//void PrintBooks(); //도서 출력
//void SaveFile(FILE* fp); //저장
//void FreeBooks(); //메모리 할당 해제
//void SearchBooks(); //도서 검색
//void SearchAuthor(); //작가명으로 검색
//void SearchBookname(); //도서명으로 검색
//void ResetFile(FILE* fp); //초기화
//void Rent(); //대출/반납
//void LoadBook(FILE* fp); // 파일에서 책 읽어오기
//void RecommendBook(); //추천 도서 목록
//
//int main() {
//    FILE* fp = fopen("books.txt", "rb");
//    if (fp == NULL) {
//        fprintf(stderr, "books.txt 파일을 찾을 수 없습니다.\n");
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
//                fprintf(stderr, "books.txt 파일을 찾을 수 없습니다.\n");
//                return;
//            }
//            ResetFile(fp);
//            printf("초기화 완료\n");
//            break;
//        case 7:
//            fp = fopen("books.txt", "wb");
//            if (fp == NULL) {
//                fprintf(stderr, "books파일을 찾을 수 없습니다.", "books.txt");
//                exit(1);
//            }
//            SaveFile(fp);
//            fclose(fp);
//            return 0;
//        }
//        printf("\n 엔터를 눌러 메뉴로 돌아갑니다");
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
//    if (fp == NULL) { // 파일이 없는 경우 초기화
//        printf("파일이 존재하지 않습니다. 새로운 데이터를 추가하세요.\n");
//        return;
//    }
//
//    int read = 0;
//    while (1) {
//        Book* book = (Book*)malloc(sizeof(Book)); // 새 도서 생성
//        if (book == NULL) {
//            fprintf(stderr, "메모리 할당 실패\n");
//            exit(1);
//        }
//        read = fread(book, sizeof(Book), 1, fp); // 읽기 시도
//        if (read != 1) { // 더 이상 읽을 데이터가 없으면 종료
//            free(book);
//            break;
//        }
//
//        if (head == NULL) { // 연결 리스트가 비어있다면 첫 노드로 설정
//            head = book;
//            book->prev = NULL;
//        }
//        else { // 기존 연결 리스트에 추가
//            tail->next = book;
//            book->prev = tail;
//        }
//        book->next = NULL;
//        tail = book;
//        cnt++;
//    }
//    fclose(fp); // 파일 닫기
//}
//
//
//int SelectMenu() {
//    Menu();
//    int menuSelect = 1; //메뉴 선택
//    int getC = 1, mY = 3;
//
//    GotoXY(6, mY);
//    printf("▶");
//    while (getC != ENTER) {
//        if (_kbhit()) { //키보드가 입력된 상태인가?
//            getC = _getch(); //누른거 아스키코드로 받음
//            switch (getC) {
//            case UP:
//                if (menuSelect > 1) {
//                    GotoXY(6, mY);
//                    printf("    ");
//                    mY -= 2;
//                    GotoXY(6, mY);
//                    printf("▶");
//                    menuSelect -= 1;
//                }
//                break;
//            case DOWN:
//                if (menuSelect < 7) {
//                    GotoXY(6, mY);
//                    printf("    ");
//                    mY += 2;
//                    GotoXY(6, mY);
//                    printf("▶");
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
//    printf("추가할 도서명을 입력해주세요 : ");
//    scanf_s("%[^\n]s", book->name, sizeof(book->name));
//    getchar();
//
//    printf("작가명을 입력해주세요 : ");
//    scanf_s("%[^\n]s", book->author, sizeof(book->author));
//    getchar();
//
//    book->searchCnt = 0; //초기 검색 횟수
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
//    printf("도서가 성공적으로 추가되었습니다.\n");
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
//        printf("도서 목록이 비어있습니다.\n");
//        return;
//    }
//
//    printf("========================도서 목록=========================\n");
//    while (tmp != NULL) {
//        printf("             도서명: %s, 작가명: %s\n", tmp->name, tmp->author);
//        printf("----------------------------------------------------------\n");
//        tmp = tmp->next;
//    }
//    int getC = 1, mY = 1;
//    int edit = 0;
//    tmp = head;
//    GotoXY(1, mY);
//    printf("▶");
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
//                    printf("▶");
//                    tmp = tmp->prev;
//                }
//                break;
//            case DOWN:
//                if (tmp->next != NULL) {
//                    GotoXY(1, mY);
//                    printf("  ");
//                    mY += 2;
//                    GotoXY(1, mY);
//                    printf("▶");
//                    tmp = tmp->next;
//                }
//                break;
//            case ENTER: //편집, 삭제
//                system("cls");
//                printf("편집을 원하시면 1, 삭제를 원하시면 2, 메뉴로 돌아가기를 원하시면 3을 입력해주세요 :");
//                scanf("%d", &edit);
//                getchar();
//
//                system("cls");
//                if (edit == 1) {
//                    printf("수정하시고싶은 책 제목을 입력해주세요 :");
//                    scanf("%s", tmp->name);
//                    getchar();
//
//                    printf("수정하시고싶은 작가명을 입력해주세요 :");
//                    scanf("%s", tmp->author);
//                    getchar();
//
//                }
//                else if (edit == 2) {
//                    printf("도서를 삭제합니다.\n");
//
//                    // 첫 번째 노드인지 확인
//                    if (tmp->prev == NULL) {
//                        head = tmp->next; // head를 다음 노드로 변경
//                        if (head != NULL) {
//                            head->prev = NULL; // 새 head의 prev를 NULL로 설정
//                        }
//                    }
//                    else {
//                        tmp->prev->next = tmp->next; // 이전 노드의 next를 현재 노드의 next로 연결
//                    }
//
//                    if (tmp->next != NULL) {
//                        tmp->next->prev = tmp->prev; // 다음 노드의 prev를 현재 노드의 prev로 연결
//                    }
//
//                    // 현재 노드의 메모리 해제
//                    free(tmp);
//
//                    printf("도서가 삭제되었습니다!\n");
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
//        printf("도서 목록이 비어있습니다.\n");
//        return;
//    }
//    printf("==================== 추천 도서 목록====================\n");
//    while (tmp != NULL) {
//        if (tmp->searchCnt > 5) {
//            printf("           도서명: %s, 작가명: %s     \n", tmp->name, tmp->author);
//            printf("--------------------------------------------------\n");
//        }
//        tmp = tmp->next;
//    }
//}
//
//void SaveFile(FILE* fp) {
//    if (fp == NULL) {
//        printf("파일 포인터가 NULL입니다. 파일을 열 수 없습니다.\n");
//        return;
//    }
//
//    book = head;
//    while (book != NULL) {
//        // fwrite를 사용하여 책 정보를 파일에 바이너리 형식으로 쓰기
//        if (fwrite(book, sizeof(Book), 1, fp) != 1) {
//            printf("파일에 쓰기 오류 발생!\n");
//            return;
//        }
//        book = book->next;  // 다음 책으로 이동
//    }
//    fclose(fp);  // 파일 닫기
//}
//
//void Rent() {
//    book = head;
//    //char bookName[100];
//    int isRent = 0;
//
//    if (book == NULL) {
//        printf("대출/반납 가능한 도서가 없습니다.\n");
//    }
//    int getC = 1, mY = 2;
//    int want = 0;
//    char bookName[100];
//    printf("도서 대출을 원하시면 1, 반납을 원하시면 2를 입력해주세요 :");
//    scanf("%d", &want);
//    getchar();
//    if (want == 2) {
//        int able = 0;
//
//        Book* tmp = head;
//        if (tmp == NULL) {
//            printf("도서 목록이 비어있습니다.\n");
//            return;
//        }
//
//        printf("=======================도서 목록============================\n");
//        while (tmp != NULL) {
//            if (tmp->rent == 0) {
//                printf("           도서명: %s, 작가명: %s, 대출 가능\n", tmp->name, tmp->author);
//                printf("------------------------------------------------------------\n");
//            }
//            else if (tmp->rent == 1) {
//                printf("           도서명: %s, 작가명: %s, 대출 불가능\n", tmp->name, tmp->author);
//                printf("------------------------------------------------------------\n");
//            }
//
//            tmp = tmp->next;
//        }
//
//        book = head;
//        GotoXY(1, mY);
//        printf("▶");
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
//                        printf("▶");
//                        book = book->prev;
//                    }
//                    break;
//                case DOWN:
//                    if (book->next != NULL) {
//                        GotoXY(1, mY);
//                        printf("  ");
//                        mY += 2;
//                        GotoXY(1, mY);
//                        printf("▶");
//                        book = book->next;
//                    }
//                    break;
//                case ENTER: //반납
//                    system("cls");
//                    if (book->rent == 1) {
//                        book->rent = 0;
//                        printf("반납되었습니다! 감사합니다.\n");
//                        break;
//                    }
//                    else {
//                        printf("대출중인 도서가 아닙니다.\n");
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
//            printf("도서 목록이 비어있습니다.\n");
//            return;
//        }
//
//        printf("=======================도서 목록============================\n");
//        while (tmp != NULL) {
//            if (tmp->rent == 0) {
//                printf("           도서명: %s, 작가명: %s, 대출 가능\n", tmp->name, tmp->author);
//                printf("------------------------------------------------------------\n");
//            }
//            else if (tmp->rent == 1) {
//                printf("           도서명: %s, 작가명: %s, 대출 불가능\n", tmp->name, tmp->author);
//                printf("------------------------------------------------------------\n");
//            }
//            
//            tmp = tmp->next;
//        }
//
//        book = head;
//        GotoXY(1, mY);
//        printf("▶");
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
//                        printf("▶");
//                        book = book->prev;
//                    }
//                    break;
//                case DOWN:
//                    if (book->next!=NULL) {
//                        GotoXY(1, mY);
//                        printf("  ");
//                        mY += 2;
//                        GotoXY(1, mY);
//                        printf("▶");
//                        book = book->next;
//                    }
//                    break;
//                case ENTER: //대출
//                    system("cls");
//                    if (book->rent == 0) {
//                        book->rent = 1;
//                        printf("대출되었습니다.\n");                        
//                    }
//                    else {
//                        printf("이미 대출된 도서입니다.\n");
//                    }
//                    break;
//                }
//            }
//        }
//    }
//    else {
//        printf("유효하지 않은 문자입니다.\n");
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
//    printf("    1. 작가명으로 찾기 \n");
//    printf("    2. 도서명으로 찾기 \n");
//    printf("    3. 메뉴로 나가기 \n");
//
//    int getC = 1, mY = 0;
//    int select = 1;
//
//    GotoXY(1, mY);
//    printf("▶");
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
//                    printf("▶");
//                    select -= 1;
//                }
//                break;
//            case DOWN:
//                if (select < 3) {
//                    GotoXY(1, mY);
//                    printf("  ");
//                    mY += 1;
//                    GotoXY(1, mY);
//                    printf("▶");
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
//    // 작가명으로 검색할 수 있는 기능 추가
//    book = head;
//    char bookAuthor[100];
//    int able = 0;
//    printf("작가명을 입력해주세요 : ");
//    scanf("%[^\n]s", bookAuthor);
//    getchar();
//
//    while (book != NULL) {
//        if (strcmp(book->author, bookAuthor) == 0) {
//            able = 1;
//            book->searchCnt += 1;
//            printf("도서명: % s, 작가명 : % s\n", book->name, book->author);
//            break;
//        }
//        book = book->next;
//    }
//    if (!able) {
//        printf("찾으시는 작가님이 없습니다.\n");
//    }
//}
//
//void SearchBookname() {
//    system("cls");
//    // 도서명으로 검색할 수 있는 기능 추가
//    book = head;
//    char bookName[100];
//    int able = 0;
//    printf("도서명을 입력해주세요 : ");
//    scanf("%[^\n]s", bookName);
//    getchar();
//
//    while (book != NULL) {
//        if (strcmp(book->name, bookName) == 0) {
//            able = 1;
//            book->searchCnt += 1;
//            printf("도서명: % s, 작가명 : % s\n", book->name, book->author);
//            break;
//        }
//        book = book->next;
//    }
//    if (!able) {
//        printf("찾으시는 도서가 없습니다.\n");
//    }
//}
//
//void ResetFile(FILE* fp) {
//    if (fp == NULL) {
//        printf("파일 열기 실패\n");
//        return;
//    }
//    
//    char confirm;
//    printf("모든 데이터를 초기화하시겠습니까? (Y/N): ");
//    scanf(" %c", &confirm);
//    if (confirm != 'Y' && confirm != 'y') {
//        printf("초기화를 취소했습니다.\n");
//        return;
//    }
//
//    fclose(fp);
//    FreeBooks();
//    head = tail = NULL;
//    printf("파일 내용이 삭제되었습니다.\n");
//}
//
//void Menu() {
//    printf("********************************************\n");
//    printf("              도서 검색 프로그램           \n");
//    printf("********************************************\n");
//    printf("           1 : 전체 도서 목록              \n");
//    printf("--------------------------------------------\n");
//    printf("           2 : 추천 도서 목록              \n");
//    printf("--------------------------------------------\n");
//    printf("           3 : 도서 추가                   \n");
//    printf("--------------------------------------------\n");
//    printf("           4 : 도서 검색                   \n");
//    printf("--------------------------------------------\n");
//    printf("           5 : 도서 대출/반납              \n");
//    printf("--------------------------------------------\n");
//    printf("           6 : 초기화                   \n");
//    printf("--------------------------------------------\n");
//    printf("           7 : 종료 및 저장                   \n");
//    printf("********************************************\n");
//}
