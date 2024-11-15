#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

#include <string.h>

struct str_t {
    char *str_ptr;
    size_t len;
};

struct str_storage_t {
    void *prev_chunk; // str_storage_t *

    size_t idx;
    size_t chunk_size;

    char *data;
};

str_storage_t *str_storage_t_ctor(const size_t chunk_size);
void str_storage_t_dtor(str_storage_t *storage);
char *get_new_str_ptr(str_storage_t **storage, const size_t len);

str_t read_text_from_file(const char *path);
bool write_text_to_file(const str_t *text, const char *path);

int get_file_sz(const char *path);
void clear_text(str_t *text, const char sims[]);

#endif // STRING_FUNCS_H
