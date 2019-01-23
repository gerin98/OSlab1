#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <limits.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/* http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param */
#define FNV_prime 16777619
#define offset_basis 2166136261U

// TODO{
//
// - INCOMPLETE, ☑ COMPLETE, ? MAYBE
//
// ☑ CHECK IF HASHTABLE IS FULL
// ? PLACE HASHTABLE SIZE IN STRUCT (might not do this to reduce memory occupied)
// ? REHASH FUNCTION IF HASHTABLE IS FULL
// ? DELETE ELEMENT FUNCTION
// ☑ SEARCH FUNCTION
// ? DOUBLE HASHING
// ☑ searching for duplicate words to increment occurrence
// ☑ CORNER CASE: where there are more words than spaces (basically checking if hashtable is full)
// ☑ CORNER CASE: try to print words that did not originally fit into the hashtable
// ☑ currently 2 strings in main, need to make it 1 (first one doesn't get passed to print function)
// ☑ currently copying all strings, may have to change strtok to a while loop for performance
// - strdup call malloc internally so fix that
/**/

/////////////////////////////////////////////////
/*            GLOBAL VARIABLES                 */
/////////////////////////////////////////////////

const int table_size = 3001;   //997 is a prime number
char* word_arrayGlobal;

/////////////////////////////////////////////////
/*                 STRUCTS                     */
/////////////////////////////////////////////////

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

/////////////////////////////////////////////////
/*          FUNCTION PROTOTYPES                */
/////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
/*       need to remove these when running test-cases      */
/////////////////////////////////////////////////////////////
struct wc *wc_init(char*, long);
void wc_output(struct wc*);
void wc_destroy(struct wc*);
/////////////////////////////////////////////////////////////
/*                 functions that I added                  */
/////////////////////////////////////////////////////////////
void insertHashtable(struct wc* hashtable, char* token);
bool check_available(struct wc*, uint32_t, uint32_t, char*);
uint32_t hash2(char*);

/////////////////////////////////////////////////
/*                FUNCTIONS                    */
/////////////////////////////////////////////////

int main() {
    char str[500] = " \"where\" A \"prime\"number (or a prime) is a natural number greater than 1 that has no positive divisors other than 1 and itself. By Euclid's theorem, there are an infinite number of prime numbers. Subsets of the prime numbers may be generated with various formulas for primes. The first 1000 primes are listed below, followed by lists of notable types of prime numbers in alphabetical order, giving their respective first terms. 1 is neither prime nor composite.";
    struct wc* myHashTable = wc_init(str, 451);
    if(myHashTable == NULL){
        return(-1);
    }

    wc_output(myHashTable);
    wc_destroy(myHashTable);

    return 0;
}

/* initialize hash table */
struct wc *wc_init(char *word_array, long size){

    //allocate memory for the hashtable
    struct wc *wc;
    wc = (struct wc*)malloc(sizeof(struct wc));
    assert(wc);

    //allocate memory for the elements in the hashtable
    wc->entry = (struct wc_entry*)calloc((size_t )table_size, sizeof(*wc->entry));
    if(wc->entry == NULL){
        printf("Calloc failed");
        return NULL;
    }

    //make the input string available globally
    char *oldstr = malloc(strlen(word_array) + 1);
    strcpy(oldstr, word_array);
    word_arrayGlobal = word_array;

    //check if calloc initializes to zero properly
    /*
        for(int j = 0; j < table_size; j++){
            printf("%d.", j);
            if(wc->entry[j].word == NULL)
                printf("NULL ");
            if(wc->entry[j].count == 0)
                printf("0 ");
            if(wc->entry[j].checked == false)
                printf("false \n");
        }
    */


    ////////////////////////////////////////////////////////////////////////

    //obtain words from the input string
//    int start = 0;
//    for(int end = 0; end < size; end++){
//
//        //check if first character is a space
//        if(isspace(word_array[start]) != 0)
//        {
//            //starting point is a white space so increment it and reset end index
//            end = start;
//            start++;
//            continue;
//        }
//
//        //check if current character is a space
//        int test = isspace(word_array[end]);
//
//        if(test == 0){
//            //no white space
//            //do nothing
//        }
//        else{
//            //white space
//            size_t size_word = (size_t)(end - start) ;
//            char* substr = malloc((size_word+1) * sizeof(char));
//            strncpy(substr, word_array+start, size_word);
//            substr[size_word] = '\0';
//
//            //insert into the hashtable
//            insertHashtable(wc, substr);
//
//            //move start index to the next character
//            start = end + 1;
//        }
//    }
//    if(start < size){
//        //last word was not obtained from word_string because no space at the end
//        size_t size_word = (size_t)(size - start);
//        char* substr = malloc((size_word+1) * sizeof(char));
//        strncpy(substr, word_array+start, size_word);
//        substr[size_word] = '\0';
//
//        //insert into the hashtable
//        insertHashtable(wc, substr);
//    }
    ////////////////////////////////////////////////////////////////////////

    //old code but currently using this

    char *token;
    char delimit[]=" \t\r\n\v\f().,!?:;\"/\\";
    token = strtok(oldstr, delimit);
    // walk through other tokens
    while( token != NULL ) {

        // hash the token
        uint32_t hash_code = hash2(token);
        hash_code = hash_code % table_size;

        if(wc->entry[hash_code].word == NULL) {      //nothing in the wc
            wc->entry[hash_code].word = strdup(token);
            wc->entry[hash_code].count++;
        }
        else if(!strcmp(wc->entry[hash_code].word, token)){     //duplicate in the wc
            wc->entry[hash_code].count++;
        }       //check corner case when key is occupied so data is placed at next available key
        else{       //some other word in the wc
            bool insert_success = check_available(wc, hash_code, hash_code, token);
            if(!insert_success){
                //ran out of space in the wc
                printf("Hashtable Full!!! \n");
                printf("Can't Insert: %s \n", token);
                return wc;
            }
        }
        token = strtok(NULL, delimit);
    }

    //free the copy of the string
    free(oldstr);

    return wc;
}

/* print the occurrences for each word in the hashtable */
void wc_output(struct wc *wc){

    char* curr_word = "";
    int curr_occurrence = 0;
    uint32_t hash_code;
    uint32_t offset = 0;
    uint32_t original_hash_code;

    int j = 0;
    while(j < table_size){

        if(wc->entry[j].word != NULL){
            curr_word = wc->entry[j].word;
            curr_occurrence = wc->entry[j].count;
            wc->entry[j].checked = true;
            printf("%s:%d\n", curr_word, curr_occurrence);
            offset = 0;
        }
        j++;
    }

/*
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
    free(oldstr);

*/

}

/* free all allocated memory */
void wc_destroy(struct wc *wc){
    for(int j = 0; j < table_size; j++){
        if(wc->entry[j].word != NULL)
            free(wc->entry[j].word);
    }
    free(wc->entry);
    free(wc);
}

/* inserts strings into the hashtable */
void insertHashtable(struct wc* hashtable, char* token){

    // hash the token
    uint32_t hash_code = hash2(token) % table_size;

    if(hashtable->entry[hash_code].word == NULL) {      //nothing in the hashtable
        hashtable->entry[hash_code].word = (token);
        hashtable->entry[hash_code].count++;
    }
    else if(!strcmp(hashtable->entry[hash_code].word, token)){     //duplicate in the hashtable
        hashtable->entry[hash_code].count++;
    }
        //checks corner case when key is occupied so data is placed at next available key
    else{
        bool insert_success = check_available(hashtable, hash_code, hash_code, token);
        if(!insert_success){
            //ran out of space in the hashtable
            printf("Hashtable Full!!! \n");
            printf("Can't Insert: %s \n", token);
            return;
        }
    }
}

/* find empty space in hashtable and insert if possible */
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

    hashtable->entry[hash_code + offset].word = (token);
    hashtable->entry[hash_code + offset].count++;

    return true;
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

/* primary hash function */
//unsigned long hash1(char *str){
//    unsigned long hash = 5381;
//    int c;
//
//    while (c = *str++)
//        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//
//    hash = hash % 1009;
//
//    return hash;
//}