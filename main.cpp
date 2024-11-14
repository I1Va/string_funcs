#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "string_funcs.h"

int main() { // EXAMPLES
    str_storage_t *storage = str_storage_t_ctor(16);

    str_t str1 = {NULL, 13};
    str1.str_ptr = get_new_str_ptr(&storage, str1.len);
    strncpy(str1.str_ptr, "pupikapi", str1.len);

    // printf("storage_idx: %lu", storage.idx);
    str_t str2 = {NULL, 4};
    str2.str_ptr = get_new_str_ptr(&storage, str2.len);
    strncpy(str2.str_ptr, "boom", str2.len);

    str_t str3 = {NULL, 10};
    str3.str_ptr = get_new_str_ptr(&storage, str3.len);
    strncpy(str3.str_ptr, "goooooool", str3.len);


    // printf("s1: '%s'\n", str1.str_ptr);
    // printf("s2: '%s'\n", str2.str_ptr);
    // printf("s3: '%s'\n", str3.str_ptr);

    // printf("FINAL: storage prev_chunk : [%p]\n", storage->prev_chunk);


    str_t text = read_text_from_file("test.txt");

    // printf("\n\n%s", text);
    clear_text(&text, "\n ");

    str_storage_t_dtor(storage);

    return 0;
}
