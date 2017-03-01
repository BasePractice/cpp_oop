#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include "c_basic.h"

/** HASH: https://gist.github.com/tonious/1377667 */

unsigned long
__ht_hash(struct hash_table *table, char *key) {

    unsigned long int val = 0;
    int i = 0;

    while (val < ULONG_MAX && i < strlen(key)) {
        val = val << 8;
        val += key[i];
        i++;
    }
    return val % table->size;
}

struct entry *
__ht_create_pair(char *key, char *value) {
    struct entry *pair;

    pair = malloc(sizeof(struct entry));
    pair->key = strdup(key);
    pair->value = strdup(value);
    pair->next = NULL;
    return pair;
}

struct hash_table *
ht_create(int size) {
    int i;
    struct hash_table *ht = (struct hash_table *) calloc(1, sizeof(struct hash_table));

    ht->size = size;
    ht->table = malloc(sizeof(struct entry *) * size);
    for (i = 0; i < size; i++) {
        ht->table[i] = NULL;
    }
    return ht;
}

void
ht_free(struct hash_table *hash_table) {

}

void
ht_put(struct hash_table *table, char *key, char *value) {
    unsigned long bin = 0;
    struct entry *pair = NULL;
    struct entry *next = NULL;
    struct entry *last = NULL;

    bin = __ht_hash(table, key);

    next = table->table[bin];

    while (next != NULL && next->key != NULL && strcmp(key, next->key) > 0) {
        last = next;
        next = next->next;
    }

    if (next != NULL && next->key != NULL && strcmp(key, next->key) == 0) {
        free(next->value);
        next->value = strdup(value);
    } else {
        pair = __ht_create_pair(key, value);

        if (next == table->table[bin]) {
            pair->next = next;
            table->table[bin] = pair;
        } else if (next == NULL && last != NULL) {
            last->next = pair;
        } else if (last != NULL) {
            pair->next = next;
            last->next = pair;
        }
    }
}


char *
ht_get(struct hash_table *table, char *key) {
    unsigned long bin = 0;
    struct entry *pair;

    bin = __ht_hash(table, key);

    pair = table->table[bin];
    while (pair != NULL && pair->key != NULL && strcmp(key, pair->key) > 0) {
        pair = pair->next;
    }

    if (pair == NULL || pair->key == NULL || strcmp(key, pair->key) != 0) {
        return NULL;
    } else {
        return pair->value;
    }
}

int
cb_file_content(const char *const file_name, unsigned char **content, size_t *content_size) {
    FILE *fd;

    fd = fopen(file_name, "rb");
    if (fd != NULL) {
        fseek(fd, 0, SEEK_END);
        (*content_size) = (size_t) ftell(fd);
        rewind(fd);
        (*content) = (unsigned char *) calloc(1, (*content_size));
        fread((*content), (*content_size), 1, fd);
        fclose(fd);
        return 1;
    }
    return 0;
}

void
cb_free_content(unsigned char **content) {
    if (content != NULL && (*content) != NULL) {
        free((*content));
        (*content) = NULL;
    }
}

void
cb_statistic_content(const unsigned char *content,
                     size_t content_size,
                     struct cb_statistic *statistic) {
    if (content != NULL) {
        size_t i;

        statistic->alphabets = 0;
        statistic->whitespaces = 0;
        statistic->digits = 0;
        statistic->symbols = 0;
        statistic->lines = 0;
        for (i = 0; i < content_size; ++i) {
            int ch = (int) content[i];
            if (isalpha(ch)) {
                statistic->alphabets++;
            } else if (isdigit(ch)) {
                statistic->digits++;
            } else if (isspace(ch)) {
                if (ch == '\n')
                    statistic->lines++;
                statistic->whitespaces++;
            } else {
                statistic->symbols++;
            }
        }
    }
}