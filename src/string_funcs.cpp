#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
