#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <limits.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

/* http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param */
#define FNV_prime 16777619
#define offset_basis 2166136261U

// TODO
//
// - INCOMPLETE, ☑ COMPLETE, ? MAYBE
//
// ☑ CHECK IF HASHTABLE IS FULL
// ? PLACE HASHTABLE SIZE IN STRUCT (might not do this to reduce memory occupied)
// ? REHASH FUNCTION IF HASHTABLE IS FULL
// ? DELETE ELEMENT FUNCTION
// ☑ SEARCH FUNCTION
// - DOUBLE HASHING
// ☑ searching for duplicate words to increment occurrence
// ☑ CORNER CASE: where there are more words than spaces (basically checking if hashtable is full)
// ☑ CORNER CASE: try to print words that did not originally fit into the hashtable
// - currently 2 strings in main, need to make it 1 (first one doesn't get passed to print function)
// - currently copying all strings, may have to change strtok to a while loop for performance

/* global functions (for testing purposes only), 997 is a prime number */
int table_size = 997;
char* word_arrayGlobal;

/* entry into hash table */
struct wc_entry{
    char* word;
    int count;
    bool checked;
};

/* hashtable */
struct wc{
    struct wc_entry* entry;
};

/* function prototypes */
struct wc *wc_init(char*, long);
bool check_available(struct wc*, uint32_t, uint32_t, char*);
void wc_output(struct wc *);
void wc_destroy(struct wc*);
unsigned long hash1(char*);
uint32_t hash2(char*);

int main() {
    char str[500] = "A prime number (or a prime) is a natural number greater than 1 that has no positive divisors other than 1 and itself. By Euclid's theorem, there are an infinite number of prime numbers. Subsets of the prime numbers may be generated with various formulas for primes. The first 1000 primes are listed below, followed by lists of notable types of prime numbers in alphabetical order, giving their respective first terms. 1 is neither prime nor composite.";

    // need to remove this later
    /*
        size_t inputSize = strlen(str);
        printf("\nInput String Size: %d\n\n", inputSize);
    */
    struct wc* myHashTable = wc_init(str, 997);
    if(myHashTable == NULL){
        return(-1);
    }

    wc_output(myHashTable);
    wc_destroy(myHashTable);

    // print out elements in the hashtable (testing purposes)
    /*
    for(int j = 0; j < table_size; j++){
        if(myHashTable->entry[j].word != NULL){
            printf("%d: %s, %d\n", j, myHashTable->entry[j].word, myHashTable->entry[j].count);
        }
    }
    */

    return 0;
}

/* initialize hash table */
struct wc *wc_init(char *word_array, long size){

    struct wc *hashtable;
    hashtable = (struct wc*)malloc(sizeof(struct wc));

    hashtable->entry = (struct wc_entry*)calloc((size_t )table_size, sizeof(*hashtable->entry));

    if(hashtable->entry == NULL){
        printf("Calloc failed");
        return NULL;
    }

    //make a copy of the string to use
    char *oldstr = malloc(strlen(word_array) + 1);
    strcpy(oldstr, word_array);
    word_arrayGlobal = word_array;

    //check if calloc initializes to zero properly
    /*
        for(int j = 0; j < table_size; j++){
            printf("%d.", j);
            if(hashtable->entry[j].word == NULL)
                printf("NULL ");
            if(hashtable->entry[j].count == 0)
                printf("0 ");
            if(hashtable->entry[j].checked == false)
                printf("false \n");
        }
    */

    char *token;
    token = strtok(oldstr, " ");
    // walk through other tokens
    while( token != NULL ) {

        // hash the token
        uint32_t hash_code = hash2(token);
        hash_code = hash_code % table_size;

        if(hashtable->entry[hash_code].word == NULL) {      //nothing in the hashtable
            hashtable->entry[hash_code].word = strdup(token);
            hashtable->entry[hash_code].count++;
        }
        else if(!strcmp(hashtable->entry[hash_code].word, token)){     //duplicate in the hashtable
            hashtable->entry[hash_code].count++;
        }       //check corner case when key is occupied so data is placed at next available key
        else{       //some other word in the hashtable
            bool insert_success = check_available(hashtable, hash_code, hash_code, token);
            if(!insert_success){
                //ran out of space in the hashtable
                printf("Hashtable Full!!! \n");
                printf("Can't Insert: %s \n", token);
                return hashtable;
            }
        }
        token = strtok(NULL, " ");
    }

    //free the copy of the string
    free(oldstr);

    return hashtable;
}

/* find empty space in hashtable */
bool check_available(struct wc *hashtable, uint32_t hash_code, uint32_t original_hash_code, char* token){

    uint32_t offset = 0;
    while(hashtable->entry[hash_code + offset].word != NULL){

        //check if the word is a repeat
        if(!strcmp(hashtable->entry[hash_code + offset].word, token)){
            hashtable->entry[hash_code + offset].count++;
            return true;
        }

        //if not, move to the next spot in the hashtable
        offset++;

        //check if new hash_code is out of bounds
        if( (hash_code + offset) >= table_size) {
            hash_code = 0;
            offset = 0;
        }

        //repeat hashtable: no more room
        if(hash_code + offset == original_hash_code){
            return false;
        }

    }

    hashtable->entry[hash_code + offset].word = strdup(token);
    hashtable->entry[hash_code + offset].count++;

    return true;
}

/* print the occurrences for each word in the hashtable */
void wc_output(struct wc *wc){

    char* curr_word = "";
    int curr_occurrence = 0;
    uint32_t hash_code;
    uint32_t offset = 0;
    uint32_t original_hash_code;

    char *oldstr = malloc(strlen(word_arrayGlobal) + 1);
    strcpy(oldstr, word_arrayGlobal);

    char *token;
    token = strtok(oldstr, " ");
    // walk through other tokens
    while( token != NULL ) {

        // hash the token
        hash_code = hash2(token) % table_size;
        original_hash_code = hash_code;

        //search for the token in the wc
        while ( (wc->entry[hash_code + offset].word != NULL) && (strcmp(wc->entry[hash_code + offset].word, token) != 0) ){
            offset++;

            //check if new hash_code is out of bounds
            if( (hash_code + offset) >= table_size) {
                hash_code = 0;
                offset = 0;
            }

            //check if every element in the wc has been checked and we are now repeating
            if(hash_code + offset == original_hash_code){
                offset = 0;
                break;
            }

        }

        if(wc->entry[hash_code + offset].word == NULL){  //probably don't need this
            // word doesn't exist in the wc
            offset = 0;
            break;
        }

        //mark as read if not read yet
        if(wc->entry[hash_code + offset].checked == false){
            curr_word = wc->entry[hash_code + offset].word;
            curr_occurrence = wc->entry[hash_code + offset].count;
            wc->entry[hash_code + offset].checked = true;
            printf("%s:%d\n", curr_word, curr_occurrence);
            offset = 0;
        }

        token = strtok(NULL, " ");
    }

}

/* free all allocated memory */
void wc_destroy(struct wc *wc){
    free(wc->entry);
    free(wc);
}

/* primary hash function */
unsigned long hash1(char *str){
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    hash = hash % 1009;

    return hash;
}

/* secondary hash function */
uint32_t hash2(char* str){

    uint32_t hash = offset_basis;
    uint32_t i;

    for(i = 0; i < strlen(str); i++){
        hash = hash ^ str[i];
        hash = hash * FNV_prime;
    }

    return hash;
}