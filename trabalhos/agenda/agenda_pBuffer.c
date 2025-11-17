#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 layout da memória (como o pbuffer é organizado):
 -----------------------------------------------------------------------------
 | op (int) | qntd (int) | pessoa 1 (94 bytes) | pessoa 2 (94 bytes) | ...
 -----------------------------------------------------------------------------
 | 4 bytes  | 4 bytes    | 40b(nome)+4b(idade)+50b(email) | ...
*/

#define NOME 40
#define IDADE sizeof(int)
#define EMAIL 50           
#define TAM_PESSOA (NOME + IDADE + EMAIL)

void *pbuffer;
void adicionarPessoa();
void removerPessoa();
void buscarPessoa();
void listarTodos();

int main() {
    
    pbuffer = malloc(2 * sizeof(int));
    if (pbuffer == NULL) {
        printf("erro fatal ao alocar memoria inicial\n");
        return 1;
    }

    //ponteiro para a opção do menu
    *(int *)pbuffer = 0;
    
    //ponteiro para a quantidade de pessoas
    *((int *)pbuffer + 1) = 0;

    //ponteiros de controle (apontam para dentro do pbuffer) ---
    
    //'op' aponta para o endereço base do pbuffer
    int *op = (int *)pbuffer;
    
    //'qntd' aponta para o endereço base + 1 * sizeof(int)
    int *qntd = (int *)pbuffer + 1;

    printf("agenda inicializada com %d pessoas.\n", *qntd);

    // loop principal do menu
    while (*op != 5) {
        printf("\n_____________________________\n");
        printf("|           menu              |\n");
        printf("| 1. adicionar pessoa         |\n");
        printf("| 2. remover pessoa           |\n");
        printf("| 3. buscar pessoa na agenda  |\n");
        printf("| 4. listar todos na agenda   |\n");
        printf("| 5. sair                     |\n");
        printf("|_____________________________|\n");
        printf("digite escolha:\n");
        
        // lê a opção e salva no local apontado por 'op'
        scanf("%d", op);
        getchar(); // limpa o '\n' deixado pelo scanf

        switch (*op) {
            case 1:
                adicionarPessoa();
                op = (int *)pbuffer;
                qntd = (int *)pbuffer + 1;
                break;

            case 2:
                removerPessoa();
                op = (int *)pbuffer;
                qntd = (int *)pbuffer + 1;
                break;

            case 3:
                buscarPessoa();
                op = (int *)pbuffer;
                qntd = (int *)pbuffer + 1;
                break;

            case 4:
                listarTodos();
                break;

            case 5:
                printf("encerrando programa\n");
                free(pbuffer); 
                exit(0);
                break;

            default:
                printf("opcao invalida! digite novamente\n");
                break;
        }
    }
    
    //este free só seria chamado se o loop terminasse (ex: *op == 5)
    //mas como o 'case 5' tem 'exit()', esta linha é redundante,
    //mas é boa prática tê-la.
    free(pbuffer);
    return 0;
}

void adicionarPessoa() {
    //1. achar o ponteiro 'qntd' dentro do pbuffer
    int *qntd = (int *)pbuffer + 1;
    
    //2. incrementar a quantidade de pessoas
    (*qntd)++;
    
    //3. realocar o buffer para caber a nova pessoa
    //tamanho novo = cabeçalho (8 bytes) + (nº de pessoas * 94 bytes)
    void *novoBuffer = realloc(pbuffer, (2 * sizeof(int)) + (*qntd * TAM_PESSOA));
    if (novoBuffer == NULL) {
        printf("erro ao realocar memoria para adicionar pessoa!\n");
        (*qntd)--; //desfaz o incremento, pois falhou
        return;
    }
    
    //4. atualizar o ponteiro global para o novo endereço
    pbuffer = novoBuffer;
    
    //5. atualizar o ponteiro 'qntd' (pois pbuffer pode ter mudado)
    qntd = (int *)pbuffer + 1;

    //6. calcular o endereço de início da nova pessoa
    //endereço = base + pula cabeçalho + pula pessoas anteriores
    void *novaPessoa = (char *)pbuffer + (2 * sizeof(int)) + (TAM_PESSOA * (*qntd - 1));

    //7. criar ponteiros que apontam para os campos dentro do bloco 'novapessoa'
    
    char *ptr_Nome = (char *)novaPessoa;
    int *ptr_Idade = (int *)((char *)ptr_Nome + NOME);
    char *ptr_Email = (char *)ptr_Nome + NOME + IDADE;

    // 8. coletar e salvar os dados diretamente nos endereços
    printf("nome:\n");
    fgets(ptr_Nome, NOME, stdin);
    ptr_Nome[strcspn(ptr_Nome, "\n")] = 0;

    printf("idade:\n");
    scanf("%d", ptr_Idade);
    getchar();

    printf("e-mail:\n");
    fgets(ptr_Email, EMAIL, stdin);
    ptr_Email[strcspn(ptr_Email, "\n")] = 0;

    printf("nova pessoa adicionada com sucesso!\n");
}

void removerPessoa() {
    // 'op' será usado para guardar o índice a remover
    int *op = (int *)pbuffer;
    int *qntd = (int *)pbuffer + 1;

    if (*qntd == 0) {
        printf("nenhuma pessoa na agenda\n");
        return;
    }
    
    //lista todos para o usuário saber qual índice escolher
    listarTodos();
    
    printf("digite o indice (1 a %d) da pessoa a ser removida:\n", *qntd);
    scanf("%d", op); //salva o índice no espaço 'op' do pbuffer
    getchar();

    if (*op < 1 || *op > *qntd) {
        printf("indice invalido\n");
        return;
    }
    
    //ajusta o índice para base 0 (ex: usuário digita 1, queremos índice 0)
    (*op)--;

    //1. calcular o endereço da pessoa a ser removida
    char *pessoaRemovida = (char *)pbuffer + (2 * sizeof(int)) + (TAM_PESSOA * (*op));
    
    //2. calcular o endereço da pessoa seguinte (a primeira a ser movida)
    char *proxPessoa = pessoaRemovida + TAM_PESSOA;
    
    //3. calcular quantos bytes precisam ser movidos
    //(total de pessoas - índice a remover - 1) * tamanho da pessoa
    size_t bytesParaMover = (*qntd - *op - 1) * TAM_PESSOA;

    //4. mover o bloco de memória
    //puxa todas as pessoas seguintes para "cima",
    if (bytesParaMover > 0) {
         memmove(pessoaRemovida, proxPessoa, bytesParaMover);
    }
   
    //5. decrementar o contador
    (*qntd)--;
    
    //6. realocar (encolher) o buffer para liberar a memória
    void *novoBuffer = realloc(pbuffer, (2 * sizeof(int)) + (*qntd * TAM_PESSOA));
    if (novoBuffer == NULL) {
        printf("erro ao reduzir o buffer, mas pessoa foi removida.\n");
    } else {
        pbuffer = novoBuffer;
    }

    printf("pessoa removida com sucesso!\n");
    return;
}

void buscarPessoa() {
    //usa 'op' como contador do loop (índice)
    int *op = (int *)pbuffer;
    int *qntd = (int *)pbuffer + 1;

    if (*qntd == 0) {
        printf("nenhuma pessoa na agenda!\n");
        return;
    }

    //1. aloca espaço temporário no fim do buffer para 'nomebuscado'
    //e 'ptr_encontrado'
    size_t tamanhoAtual = (2 * sizeof(int)) + (*qntd * TAM_PESSOA);
    void *novoBuffer = realloc(pbuffer, tamanhoAtual + NOME + sizeof(int));
    if (novoBuffer == NULL) {
        printf("erro ao alocar memoria para busca\n");
        return;
    }
    pbuffer = novoBuffer;

    //2. re-aponta os ponteiros de controle
    op = (int *)pbuffer;
    qntd = (int *)pbuffer + 1;

    //3. define os ponteiros para os dados temporários
    char *nomeBuscado = (char *)pbuffer + tamanhoAtual;
    int *ptr_Encontrado = (int *)((char *)nomeBuscado + NOME);
    *ptr_Encontrado = 0;

    printf("digite o nome da pessoa a ser buscada:\n");
    fgets(nomeBuscado, NOME, stdin);
    nomeBuscado[strcspn(nomeBuscado, "\n")] = 0;

    //4. loop de busca
    for (*op = 0; *op < *qntd; (*op)++) {
        //encontra o nome da pessoa atual
        char *pessoaAtual_Nome = (char *)pbuffer + (2 * sizeof(int)) + ((*op) * TAM_PESSOA);

        if (strcmp(pessoaAtual_Nome, nomeBuscado) == 0) {
            printf("pessoa encontrada!\n");
            printf(" %s esta no indice %d\n", pessoaAtual_Nome, *op + 1);
            (*ptr_Encontrado)++;
        }
    }

    if (*ptr_Encontrado == 0) {
        printf("pessoa nao encontrada.\n");
    }

    //5. libera a memória temporária usada pela busca
    //(encolhe o buffer de volta ao tamanho original)
    void *bufferReduzido = realloc(pbuffer, tamanhoAtual);
    if (bufferReduzido == NULL) {
        printf("nao foi possivel reduzir a memoria apos a busca\n");
    } else {
        pbuffer = bufferReduzido;
    }
    
    return;
}

void listarTodos() {
    int *op = (int *)pbuffer;
    int *qntd = (int *)pbuffer + 1;

    if (*qntd == 0) {
        printf("nenhuma pessoa na agenda!\n");
        return;
    }

    printf("\n_______________________________________\n");
    printf("\n     lista de contatos: %d pessoas  \n", *qntd);

    for (*op = 0; *op < *qntd; (*op)++) {
        
        //1. calcula o endereço base da pessoa 'op'
        char *pessoa = (char *)pbuffer + (2 * sizeof(int)) + (TAM_PESSOA * (*op));
        
        //2. define os ponteiros para os campos dentro dessa pessoa
        char *ptr_Nome = pessoa;
        int *ptr_Idade = (int *)(ptr_Nome + NOME);
        char *ptr_Email = (char *)(ptr_Nome + NOME + IDADE);

        //3. imprime os dados
        printf("-------- pessoa %d --------\n", *op + 1);
        printf("nome: %s\n", ptr_Nome);
        printf("idade: %d\n", *ptr_Idade);
        printf("e-mail: %s\n", ptr_Email);
        printf("--------------------------\n");
    }
    
    printf("\n_______________________________________\n");
    return;
}