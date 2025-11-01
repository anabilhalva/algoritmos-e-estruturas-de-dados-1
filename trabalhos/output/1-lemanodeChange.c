#include <stdio.h>
#include <stdbool.h>

bool lemonadeChange(int* bills, int billsSize) {
    int count5=0; //contador de notas de 5
    int count10=0; //contador de notas de 10

    for(int i=0; i < billsSize; i++){
        int payment = bills[i];

        if (payment == 5){
            count5++;
        } 
            else 
            
        if (payment == 10){
            
            if(count5 >= 1){
                count5--; //deu troco com uma nota de 5
                count10++; //recebeu uma nota de 10
            } 
            else {
                return false; //se nao tiver nota de 5 para dar troco, falha
            }
        }

            else 
            
        if(payment == 20){
            //cliente paga 20, precisa dar 15 de troco

            if(count10 >= 1 && count5 >= 1){
                count10--; //deu troco com uma nota de 10
                count5--; //deu troco com uma nota de 5
            } 
            else 
                if(count5 >= 3){
                    count5 -= 3; //remove tres notas de 5
                } 
                else {
                    return false; //se nenhuma das opcoes for possivel
                }
        }
    }

    return true; //se passamos por todos os clientes com sucesso, retorna verdadeiro
    
}
int main() {

    //exemplo 1
    int bills1[] = {5, 5, 5, 10, 20}; 
    int size1 = sizeof(bills1) / sizeof(bills1[0]); //calcula o tamanho do array
    printf("Exemple 1: %s\n", lemonadeChange(bills1, size1) ? "true" : "false"); //inicia a impressao e chama a funcao para verificar se o troco pode ser dado
    
    //exemplo 2
    int bills2[] = {5, 5, 10, 10, 20};
    int size2 = sizeof(bills2) / sizeof(bills2[0]);
    printf("Exemple 2: %s\n", lemonadeChange(bills2, size2) ? "true" : "false");

    return 0;
}
