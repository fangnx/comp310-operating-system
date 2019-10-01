#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {

    int * p;
    int i = 0;
    while (1) {
        if ((p = (int*)calloc(10, sizeof(int))) != NULL) {
            printf("Malloc #%d!\n", i);
        } else {
            printf("Malloc didn't work :(");
            break;        
        }
        i++; 
    }
}