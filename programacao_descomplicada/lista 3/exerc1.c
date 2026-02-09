#include <stdio.h>

int main(){
    int mult;

    for(int i = 1; i <= 5; i++){
        mult = i * 3;
    }

    printf("Os cinco primeros multiplos de 3 sao: %d", mult);
}