/**Faca um programa que determine o mostre os cinco primeiros multiplos de 3, conside- ´
rando numeros maiores que 0**/

#include <stdio.h>
int main() {
    int i;

    printf("Os cincos primeiros multiplos de 3 sao:\n");
    for(i = 1; i <= 5; i++) {
        printf("%d\n", i * 3);
    }
    return 0;
       
}
