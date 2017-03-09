#ifndef CPP_OOP_C_BASIC_H
#define CPP_OOP_C_BASIC_H

#if defined(__cplusplus)
extern "C" {
#endif

struct cb_statistic {
    int alphabets;
    int whitespaces;
    int digits;
    int symbols;
    int lines;
};

struct entry {
    char *key;
    char *value;
    struct entry *next;
};

struct hash_table {
    int size;
    struct entry **table;
};

struct hash_table *ht_create(int size);
void ht_put(struct hash_table *table, const char *key, const char *value);
char *ht_get(struct hash_table *table, const char *key);
void ht_free(struct hash_table *hash_table);


size_t cb_file_content(const char *const file_name, unsigned char **content, size_t *content_size);
void cb_free_content(unsigned char **content);
void cb_statistic_content(const unsigned char *content, size_t content_size, struct cb_statistic *statistic);

#if defined(__cplusplus)
}
#endif

#endif //CPP_OOP_C_BASIC_H
