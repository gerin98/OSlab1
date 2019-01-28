#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int factorial(int n){
        if(n == 0 || n == 1)
                return 1;
        else
                return n * factorial(n-1);
}


int main(int argc, char **argv) {

        char* input = argv[1];   //first input
        int size = strlen(input);   //size of the first input

        //string is empty
        if(size == 0){
                printf("Huh?\n");
                return 0;
        }

        //check for integer
        for(int j = 0; j < size; j++){
                if(input[j] > '9' || input[j] < '0'){
                        printf("Huh?\n");
                        return 0;
                }
        }

        //get the integer
        int num = atoi(input);

        if(num == 0){
                printf("Huh?\n");
                return 0;
        }

        //check if integer is greater than 12
        if(num > 12){
                printf("Overflow\n");
                return 0;
        }

        int result = factorial(num);
        printf("%d\n", result);

        return 0;
}
