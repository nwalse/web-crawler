//BY:NEEL WALSE
//CSE13s PUSUKURI PA6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crawler.h"
#define TABLE_SIZE 20000


//webpage
webpage_t *webpage_new(){
	webpage_t *page = malloc(sizeof(webpage_t));
	if (page == NULL) {
        return NULL;
    }
	page->url = NULL;
	page->html = NULL;
    page->length = 0;
    page->depth = 0;	
	
	return page;
}


void webpage_delete(webpage_t *page){
	if (page == NULL) {
        return;
    }

	free(page->url);
	free(page->html);
	free(page);
}

void webpage_print(webpage_t *item) {
    printf("URL: %s\nHTML: %s\nLength: %zu\nDepth: %d\n", item->url, item->html, item->length, item->depth);
}


//set
set_t *set_new(void) {
    set_t *set = malloc(sizeof(set_t));
    if (set == NULL) {
	return NULL;
    }    
    set->next = NULL;
    set->key = NULL;
    set->item = NULL;

    return set;
}

bool set_insert(set_t *set, const char *key, void *item) {
    if (set == NULL || key == NULL || item == NULL || set_find(set, key) != NULL) {
        return false;
    }

    set_t *t = set;
    while (t->next != NULL) {
    	t = t->next; 
    }

    t->key = strdup(key);
    t->item = item;
    t->next = set_new();
   
    return true;
}

void *set_find(set_t *set, const char *key) {
    if (set == NULL || key == NULL) {
        return NULL;
    }

    set_t *t = set;
    while (t->next != NULL) {
        if (strcmp(t->key, key) == 0)
            return t->item;
        t = t->next;
    }

    return NULL;
}

void set_print(set_t *set) {
	if (set == NULL) {
        return;
    }
	
	set_t *t = set;
    while (t->next != NULL) {
    	webpage_print(t->item);    
        t = t->next;
    }
    
}

void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (set == NULL || itemfunc == NULL) {
        return;
    }

    set_t *t = set;
    while (t->next != NULL && t->key != NULL) {
        itemfunc(arg, t->key, t->item);
        t = t->next;
    }
}


void set_delete(set_t *set) {
    if (set == NULL) {
        return;
    }

    set_t *t = set;
    while (t != NULL) {
        set_t *next = t->next;
		webpage_delete(t->item);
        free(t->key);
        free(t);
        t = next;
    }    
}


//hashtable
unsigned int hash(char* key) {
    unsigned int hash = 0;
    while (*key != '\0') {
        hash = (hash * 31) + *key;
        key++;
    }
    return hash % TABLE_SIZE;
}


hashtable_t* hashtable_create(){
    hashtable_t* ht = malloc(sizeof(hashtable_t));
    ht->entries = calloc(TABLE_SIZE, sizeof(Entry*));
    return ht;
}


void hashtable_insert(hashtable_t* ht, char* key, char* value) {
    unsigned int index = hash(key);
    
    
    Entry* entry = malloc(sizeof(Entry));
    entry->key = strdup(key);
    entry->value = strdup(value);
    
    
    ht->entries[index] = entry;
}


char* hashtable_find(hashtable_t* ht, char* key){
    unsigned int index = hash(key);
    Entry* entry = ht->entries[index];
    
    
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry++;
    }
    
    
    return NULL;
}


void hashtable_delete(hashtable_t* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry* entry = ht->entries[i];
        if (entry != NULL) {
            free(entry->key);
            free(entry->value);
            free(entry);
        }
    }
    free(ht->entries);
    free(ht);
}

void hashtable_print(hashtable_t* ht) {
    if (ht == NULL) {
        return;
    }
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry* entry = ht->entries[i];
        if (entry != NULL) {
            printf("Index: %d\n", i);
            printf("Key: %s\n", entry->key);
            printf("Value: %s\n", entry->value);
            printf("\n");
        }
    }
}

