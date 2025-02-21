//#include <stdio.h>
//#include "parson.h"
//
//int main() {
//    // JSON 파싱
//    JSON_Value* rootValue = json_parse_file("Book.json");
//    if (rootValue == NULL) {
//        printf("Failed to parse JSON.\n");
//        return 1;
//    }
//    // JSON 배열 가져오기
//    JSON_Array* array = json_value_get_array(rootValue);
//    if (array == NULL) {
//        printf("Failed to get JSON array.\n");
//        json_value_free(rootValue);
//        return 1;
//    }
//
//    // 배열의 길이 확인
//    size_t array_size = json_array_get_count(array);
//    for (size_t i = 0; i < array_size; i++) {
//        JSON_Object* obj = json_array_get_object(array, i);
//        if (obj != NULL) {
//            const char* title = json_object_get_string(obj, "Title");
//            const char* author = json_object_get_string(obj, "Author");
//            printf("Title: %s, Author: %s\n", title, author);
//        }
//    }
//    // 메모리 해제
//    json_value_free(rootValue);
//
//    return 0;
//}
