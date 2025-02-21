//#include <stdio.h>
//#include "parson.h"
//
//int main() {
//    // JSON �Ľ�
//    JSON_Value* rootValue = json_parse_file("Book.json");
//    if (rootValue == NULL) {
//        printf("Failed to parse JSON.\n");
//        return 1;
//    }
//    // JSON �迭 ��������
//    JSON_Array* array = json_value_get_array(rootValue);
//    if (array == NULL) {
//        printf("Failed to get JSON array.\n");
//        json_value_free(rootValue);
//        return 1;
//    }
//
//    // �迭�� ���� Ȯ��
//    size_t array_size = json_array_get_count(array);
//    for (size_t i = 0; i < array_size; i++) {
//        JSON_Object* obj = json_array_get_object(array, i);
//        if (obj != NULL) {
//            const char* title = json_object_get_string(obj, "Title");
//            const char* author = json_object_get_string(obj, "Author");
//            printf("Title: %s, Author: %s\n", title, author);
//        }
//    }
//    // �޸� ����
//    json_value_free(rootValue);
//
//    return 0;
//}
