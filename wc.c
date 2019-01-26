#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include <malloc.h>
#include <limits.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define FNV_prime 16777619
#define offset_basis 2166136261U
  
/////////////////////////////////////////////////
/*            GLOBAL VARIABLES                 */
/////////////////////////////////////////////////

int table_size = 48611;   //997, 16001,19181 is a prime number
//char* word_arrayGlobal;

/////////////////////////////////////////////////
/*                 STRUCTS                     */
/////////////////////////////////////////////////

/* entry into hash table */
struct wc_entry{
    char* word;
    int count;
    struct wc_entry* next;
};

/* hashtable */
struct wc {
    /* you can define this struct to have whatever fields you want. */
    struct wc_entry* entry;
};


/////////////////////////////////////////////////
/*          FUNCTION PROTOTYPES                */
/////////////////////////////////////////////////

//unsigned long hash1(char*);
unsigned long hash2(char *);
void insertHashtable(struct wc*, char*);


/////////////////////////////////////////////////
/*                FUNCTIONS                    */
/////////////////////////////////////////////////

/* initialize hash table */
struct wc *wc_init(char *word_array, long size){
    //allocate memory for the hashtable
    struct wc *wc;
    wc = (struct wc*)malloc(sizeof(struct wc));
    assert(wc);

    //allocate memory for the elements in the hashtable
    wc->entry = (struct wc_entry*)calloc((size_t )table_size, sizeof(*wc->entry));
    if(wc->entry == NULL){
//        printf("Calloc failed");
        return NULL;
    }

    //obtain words from the input string
    int start = 0;
    for(int end = 0; end < (size-1); end++){   //size -1 because last element is null space

        //check if first character is a space
        if(isspace(word_array[start]) != 0)
        {
            //starting point is a white space so increment it and reset end index
            end = start;
            start++;
            continue;
        }


        //check if current character is a space
        int test = isspace(word_array[end]);

//        if(test == 0){
//            //no white space
//
//        }
        if(test != 0){
            //white space
            int size_word = end - start ;
            char* substr = malloc((size_word+1) * sizeof(char));
            strncpy(substr, word_array+start, size_word);
            substr[size_word] = '\0';

            //insert into the hashtable
            insertHashtable(wc, substr);

            //move start index to the next character
            start = end + 1;
        }
    }
    if(start < (size-1)){   //size -1 because last element is null space
        //last word was not obtained from word_string because no space at the end

        int size_word = (size-1) - start ;
        char* substr = malloc((size_word+1) * sizeof(char));
        size_t sizet = (size_t)size_word;
        strncpy(substr, word_array+start, sizet);
        substr[size_word] = '\0';

        //insert into the hashtable
        insertHashtable(wc, substr);

    }


    return  wc;
}

void wc_output(struct wc* wc){

    for(int j = 0; j < table_size; j++){
        struct wc_entry* curr = &(wc->entry[j]);
        while(curr != NULL){
            if(curr->word != NULL){
                printf("%s:%d\n", curr->word, curr->count);
            }
            curr = curr->next;
        }
    }
}

void
wc_destroy(struct wc *wc)
{
    struct wc_entry* curr;
    struct wc_entry* next;
    for(int j = 0; j < table_size; j++){
        curr = (&(wc->entry[j]))->next;
//        curr = curr->next;
        while(curr != NULL){
            next = curr->next;
            //delete strings in nodes in linked list
            free(curr->word);
            //delete nodes in linked list in hashtable
            free(curr);
            curr = next;
        }
    }

    //delete linked lists in hashtable
    free(wc->entry);
    //delete pointer to hashtable
    free(wc);
}

/* inserts strings into the hashtable */
void insertHashtable(struct wc* wc, char* word){

    unsigned long hash_code = hash2(word);
    struct wc_entry* curr;

    // 1 or 0 items in linked list
    if(wc->entry[hash_code].next == NULL) {
        //0 items in linked list
        if (wc->entry[hash_code].word == NULL) {
            curr = &(wc->entry[hash_code]);
            curr->word = word;
            curr->count++;
        }
            //1 item in linked list
        else if(strcmp(wc->entry[hash_code].word, word) == 0){
            //word is already present
            wc->entry[hash_code].count++;
        }
            //1 item in linked list
        else{
            //word was not already present
            curr = (struct wc_entry*)calloc(1, sizeof(struct wc_entry));
            curr->word = word;
            curr->count++;
            curr->next=NULL;
            wc->entry[hash_code].next = curr;
        }
    }
        //>1 item in linked list
    else{
        //find the last node
        struct wc_entry* lastnode = &(wc->entry[hash_code]);
//        lastnode = &(wc->entry[hash_code]);
        while(lastnode->next != NULL ){
            if(strcmp(lastnode->word, word) == 0){
                lastnode->count++;
                return;
            }
            lastnode = lastnode->next;
        }
        if(strcmp(lastnode->word, word) == 0){
            lastnode->count++;
            return;
        }
        curr = (struct wc_entry*)calloc(1, sizeof(struct wc_entry));
        curr->word = word;
        curr->count++;
        curr->next=NULL;
        lastnode->next = curr;
    }

}

/* primary hash function */
//unsigned long hash1(char *str){
//    unsigned long hash = 5381;
//    int c;
//
//    while ((c = *str++))
//        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//
//    unsigned long newHash = (unsigned long) (hash) % table_size;
//    return newHash;
//}

/* secondary hash function */
unsigned long hash2(char *str) {

    uint32_t hash = offset_basis;
    uint32_t i;

    for (i = 0; i < strlen(str); i++) {
        hash = hash ^ str[i];
        hash = hash * FNV_prime;
    }
    hash = (unsigned long) hash % table_size;
    return hash;
}

