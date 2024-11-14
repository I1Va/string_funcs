#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "string_funcs.h"
#include "general.h"

str_storage_t *str_storage_t_ctor(const size_t chunk_size) {
    str_storage_t *storage = (str_storage_t *) calloc(1, sizeof(str_storage_t));

    storage->idx = 0;
    storage->chunk_size = chunk_size;
    storage->prev_chunk = NULL;

    storage->data = (char *) calloc(chunk_size, sizeof(char));
    if (storage->data == NULL) {
        debug("storage->data calloc failed")
        return NULL;
    }

    return storage;
}

void str_storage_t_dtor(str_storage_t *storage) {
    assert(storage != NULL);

    if (storage->prev_chunk == NULL) {
        assert(storage->data != NULL);
        FREE(storage->data)
        FREE(storage)
        return;
    }

    str_storage_t_dtor((str_storage_t *) storage->prev_chunk);
}

char *get_new_str_ptr(str_storage_t **storage, const size_t len) {
    assert(storage != NULL);

    if (len + 1 > (*storage)->chunk_size) {
        debug("string doesn't fit into chunk. chunk_size: {%lu}, string_len: {%lu}", (*storage)->chunk_size, len)
        return NULL;
    }

    if ((*storage)->idx + len + 1 >= (*storage)->chunk_size) {
        void *prev_chunk = *storage;

        *storage = str_storage_t_ctor((*storage)->chunk_size);
        (*storage)->prev_chunk = prev_chunk;
        printf("storage [%p] prev_chunk: [%p]\n", storage, (*storage)->prev_chunk);

        return get_new_str_ptr(storage, len);
    }

    char *string_ptr = &((*storage)->data[(*storage)->idx]);
    (*storage)->idx += len + 1;

    return string_ptr;
}

int get_file_sz(const char *path) {
    struct stat buf = {};
    if (stat(path, &buf) != 0) {
        debug("stat error")
        return -1;
    }

    return (int) buf.st_size;
}

str_t read_text_from_file(const char *path) {
    int file_sz = get_file_sz(path);
    str_t str = {};

    if (file_sz < 0) {
        debug("file_sz invalid value : {%d}", file_sz)
        return {};
    }

    str.len = (size_t) file_sz;

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        debug("file '%s' open failed", path)
        return {};
    }

    str.str_ptr = (char *) calloc((size_t) file_sz + 1, sizeof(char)); // save one byte for '\0'

    size_t fread_cnt = fread(str.str_ptr, sizeof(char), (size_t) file_sz, file);
    assert(fread_cnt == (size_t) file_sz);

    if (fclose(file)) {
        debug("file '%s' close failed", path)
        return {};
    }
    return str;
}

bool write_text_to_file(const str_t *text, const char *path) {
    assert(text != NULL);
    assert(path != NULL);

    FILE *output_file_ptr = fopen(path, "wb");
    if (output_file_ptr == NULL) {
        debug("file '%s' open failed", path);
        return false;
    }

    fprintf(output_file_ptr, "%s", text->str_ptr);
    if (fclose(output_file_ptr)) {
        debug("file '%s' close failed", path)
        return false;
    }

    return true;
}

void clear_text(str_t *text, const char sims[]) {
    size_t left = 0;
    size_t right = 0;

    while (right < text->len) {
        if (strchr(sims, text->str_ptr[right]) == NULL) {
            text->str_ptr[left] = text->str_ptr[right];
            left++;
            right++;
        } else {
            right++;
        }
    }

    while (left < text->len) {
        text->str_ptr[left++] = '\0';
    }

    printf("[%s]", text->str_ptr);
}
