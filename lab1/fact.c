#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int factorial(int n){
    if(n == 0 || n == 1)
        return 1;
    else
        return n * factorial(n-1);
}

int main(int argc, char *argv[]) {

    char* arg[] = {"3",
                   "6"};

    char* input = arg[0];   //first input
    int size = strlen(input);   //size of the first input

//    //string is empty
//    if(arg[0][0] == '\0'){
//        printf("Huh?");
//        return 0;
//    }

    //string is empty
    if(size == 0){
        printf("Huh?");
        return 0;
    }

    //check for integer
    for(int j = 0; j < size; j++){
        if(input[j] > '9' || input[j] < '0'){
            printf("Huh?");
            return 0;
        }
    }

    //get the integer
    int num = atoi(input);

    //check if integer is greater than 12
    if(num > 12){
        printf("Overflow");
        return 0;
    }

    int result = factorial(num);
    printf("%d", result);

//    double input = atof(arg[0]);
//    printf("%f", input);

//    if(strlen(arg[0]) > 1){
//        printf("size greater than 1");
//        return 0;
//    }
//    else if( c < '1' || c > '9'){
//        printf("does not start with an integer");
//        return 0;
//    }
//    else if(){
//
//    }
//
//
//    if(arg[0][1] != '\0') {
//        printf("not an integer input");
//        return 0;
//    }
//
//    int x = c - '0';
//    int fact = factorial(x);
//    printf("%d", fact);

    return 0;
}
