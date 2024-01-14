#ifndef CRAWLER_H
#define CRAWLER_H
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
	char *url;
	char *html;
	size_t length;
	int depth;
} webpage_t;


webpage_t *webpage_new();
void webpage_delete(webpage_t *page);
void webpage_print(webpage_t *item);




typedef struct set_t set_t;
struct set_t {
    char *key;
    void *item;
    set_t *next;
};

set_t* set_new(void);
bool set_insert(set_t *set, const char *key, void *item);
void *set_find(set_t *set, const char *key);
void set_print(set_t *set);
void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item) );
void set_delete(set_t *set);





typedef struct {
    char* key;
    char* value;
} Entry;

typedef struct {
    Entry** entries;
} hashtable_t;

unsigned int hash(char* key);
hashtable_t* hashtable_create();
void hashtable_insert(hashtable_t* ht, char* key, char* value);
char* hashtable_find(hashtable_t* ht, char* key);
void hashtable_delete(hashtable_t* ht);
void hashtable_print(hashtable_t* ht);
#endif
