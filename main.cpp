#include <assert.h>
#include <cstdlib>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "string_funcs.h"


void get_string(FILE *stream, char *bufer, str_storage_t **storage) {
    assert(stream != NULL);
    assert(bufer != NULL);
    assert(storage != NULL);

    int str_len = 0;

    fscanf(stream, "%s%n", bufer, &str_len);

    printf("name : '%s', cnt : {%d}\n", bufer, str_len);
}

void test1() {
    str_storage_t *storage = str_storage_t_ctor(16); // FIXME: рекурсия плохо

    str_t str1 = {get_new_str_ptr(&storage, 13), 13};
    strncpy(str1.str_ptr, "pupikapi", str1.len + 1);

    // printf("storage_idx: %lu", storage.idx);
    str_t str2 = {get_new_str_ptr(&storage, 4), 4};
    strncpy(str2.str_ptr, "boom", str2.len + 1);

    str_t str3 = {NULL, 10};
    str3.str_ptr = get_new_str_ptr(&storage, str3.len);
    strncpy(str3.str_ptr, "goooooool", str3.len);


    // printf("s1: '%s'\n", str1.str_ptr);
    // printf("s2: '%s'\n", str2.str_ptr);
    // printf("s3: '%s'\n", str3.str_ptr);

    // printf("FINAL: storage prev_chunk : [%p]\n", storage->prev_chunk);


    str_t text = read_text_from_file("test.txt");

    // printf("\n\n%s", text);
    remove_chars_from_text(&text, "\n ");

    str_storage_t_dtor(storage);
}

void string_storage_test() {
    size_t chunk_size = 32;

    str_storage_t *storage = str_storage_t_ctor(chunk_size);

    for (size_t i = 0; i < 10; i++) {
        size_t len =  ((size_t) rand()) % (chunk_size - 2);
        printf("len 1212: {%lu}\n", len);
        get_new_str_ptr(&storage, len);
    }

    str_storage_t_dtor(storage);
}

int main() { // EXAMPLES
    size_t chunk_size = 32;

    str_storage_t *storage = str_storage_t_ctor(chunk_size);

    for (size_t i = 0; i < 100; i++) {
        size_t len =  ((size_t) rand()) % (chunk_size - 2);
        printf("len : {%lu}\n", len);
        get_new_str_ptr(&storage, len);
    }

    printf("main : storage: [%p],  prev: [%p]\n", storage, storage->prev_chunk);
    str_storage_t_dtor(storage);
}
