#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <mem.h>
#include <ctype.h>

#define FNV_prime 16777619
#define offset_basis 2166136261U

const unsigned long table_size = 16001;   //997, 3001, 6101 is a prime number

struct wc_entry {
    char *word;
    int count;
    struct wc_entry *next;
};

struct wc {
    struct wc_entry *entry;
};

//110458759 words
//55166066

struct wc *wc_init(char *, long);
void wc_output(struct wc *);
void wc_destroy(struct wc *);
unsigned long hash1(char *);
unsigned long hash2(char *);

void insertHashtable(struct wc *, char *);

int main() {
//    char word_array[30] = "this is a test string";
    char word_array[492] = "Professor: Mostly ease of use: the OS will give each program the view that it\n"
                           "has a large contiguous address space to put its code and data into; thus, as a\n"
                           "programmer, you never have to worry about things like “where should I store this\n"
                           "variable?” because the virtual address space of the program is large and has\n"
                           "lots of room for that sort of thing. Life, for a programmer, becomes much more\n"
                           "tricky if you have to worry about fitting all of your code data into a small,\n"
                           "crowded memory.";
    struct wc *myHashTable = wc_init(word_array, 492);
    if (myHashTable == NULL) {
        return (-1);
    }

    wc_output(myHashTable);
    wc_destroy(myHashTable);

    return 0;
}

struct wc *wc_init(char *word_array, long size) {
    //allocate memory for the hashtable
    struct wc *wc;
    wc = (struct wc *) malloc(sizeof(struct wc));
    assert(wc);

    //allocate memory for the elements in the hashtable
    wc->entry = (struct wc_entry *) calloc((size_t) table_size, sizeof(*wc->entry));
    if (wc->entry == NULL) {
//        printf("Calloc failed");
        return NULL;
    }

    //obtain words from the input string
    int start = 0;
    for (int end = 0; end < (size - 1); end++) {   //size -1 because last element is null space

        //check if first character is a space
        if (isspace(word_array[start]) != 0) {
            //starting point is a white space so increment it and reset end index
            end = start;
            start++;
            continue;
        }

        //check if current character is a space
        int test = isspace(word_array[end]);

        if (test == 0) {

        } else {
            //white space
            int size_word = end - start;
            char *substr = malloc((size_word + 1) * sizeof(char));
            strncpy(substr, word_array + start, size_word);
            substr[size_word] = '\0';

            //insert into the hashtable
            insertHashtable(wc, substr);

            //move start index to the next character
            start = end + 1;
        }
    }
    if (start < (size - 1)) {   //size -1 because last element is null space
        //last word was not obtained from word_string because no space at the end

        int size_word = (size - 1) - start;
        char *substr = malloc((size_word + 1) * sizeof(char));
        size_t sizet = (size_t) size_word;
        strncpy(substr, word_array + start, sizet);
        substr[size_word] = '\0';

        //insert into the hashtable
        insertHashtable(wc, substr);

    }

    return wc;
    //test
    /*
    wc->entry[5].word = "hi";
    wc->entry[5].count = 4;

    struct wc_entry* curr;
    curr = (struct wc_entry*)calloc(1, sizeof(struct wc_entry));
    curr->word = "there";
    curr->count = 2;
    wc->entry[5].next = curr;



    //check if calloc initializes to zero properly

        for(int j = 0; j < table_size; j++){
            printf("%d. ", j);
            if(wc->entry[j].word == NULL)
                printf("NULL ");
            else
                printf("%s ",wc->entry[j].word);

            struct wc_entry* curr2 = &(wc->entry[j]);
            while(curr2->next != NULL){
                curr2 = curr2->next;
                printf("(%s ", curr2->word);
                printf("%d) ", curr2->count);
            }
            printf("%d\n",wc->entry[j].count);
        }
*/


}

void insertHashtable(struct wc *wc, char *word) {

    unsigned long hash_code = hash2(word);
    struct wc_entry *curr;

    // 1 or 0 items in linked list
    if (wc->entry[hash_code].next == NULL) {
        //0 items in linked list
        if (wc->entry[hash_code].word == NULL) {
            curr = &(wc->entry[hash_code]);
            curr->word = word;
            curr->count++;
        }
            //1 item in linked list
        else if (strcmp(wc->entry[hash_code].word, word) == 0) {
            //word is already present
            wc->entry[hash_code].count++;
        }
            //1 item in linked list
        else {
            //word was not already present
            curr = (struct wc_entry *) calloc(1, sizeof(struct wc_entry));
            curr->word = word;
            curr->count++;
            curr->next = NULL;
            wc->entry[hash_code].next = curr;
        }
    }
        //>1 item in linked list
    else {
        //find the last node
        struct wc_entry *lastnode;
        lastnode = &(wc->entry[hash_code]);
        while (lastnode->next != NULL) {
            if (strcmp(lastnode->word, word) == 0) {
                lastnode->count++;
                return;
            }
            lastnode = lastnode->next;
        }
        if (strcmp(lastnode->word, word) == 0) {
            lastnode->count++;
            return;
        }
        curr = (struct wc_entry *) calloc(1, sizeof(struct wc_entry));
        curr->word = word;
        curr->count++;
        curr->next = NULL;
        lastnode->next = curr;
    }
//    printf("%s\n",wc->entry[hash_code].word);
}

void wc_output(struct wc *wc) {

    for (int j = 0; j < table_size; j++) {
        struct wc_entry *curr = &(wc->entry[j]);
        while (curr != NULL) {
            if (curr->word != NULL) {
                printf("%s:%d\n", curr->word, curr->count);
            }
            curr = curr->next;
        }
    }
}

void wc_destroy(struct wc *wc) {

    struct wc_entry *curr;
    struct wc_entry *next;
    for (int j = 0; j < table_size; j++) {
        curr = &(wc->entry[j]);
        curr = curr->next;
        while (curr != NULL) {
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

/* secondary hash function */
unsigned long hash2(char *str) {

    uint32_t hash = offset_basis;
    uint32_t i;

    for (i = 0; i < strlen(str); i++) {
        hash = hash ^ str[i];
        hash = hash * FNV_prime;
    }

    unsigned long newHash = (unsigned long) (hash) % table_size;
    return newHash;
}

/* primary hash function */
unsigned long hash1(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    hash = hash % table_size;

    return hash;
}

