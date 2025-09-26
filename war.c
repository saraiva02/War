#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definição da struct Territorio
// Esta estrutura agrupa dados relacionados (nome, cor, tropas) em uma única unidade.

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} territorio;

int main(){

     // Declaração do vetor de structs.
     // territorios é um array que pode armazenar 5 estruturas do tipo Territorio.

    territorio territorios[5];
    int i;

    printf("--- Cadastro de Territorios ---\n" );

    printf("\n-----------------------------\n");
    printf("-----------------------------\n");

    // Laço de entrada de dados.
    // Este laço 'for' se repete 5 vezes para coletar informações de cada território.

    for (i = 0; i < 5; i++) {

        printf("\nTerritorio %d:\n", i + 1);

        // Leitura do nome usando fgets
        printf("Nome do territorio: ");
        fgets(territorios[i].nome, 30, stdin);
        // Remove o '\n' extra do final da string
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0;
        
        // Leitura da cor usando fgets
        printf("Cor do exercito: ");
        fgets(territorios[i].cor, 10, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = 0;
        
        // Leitura das tropas com scanf
        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        
        // Limpa o buffer de entrada após a leitura do número
        while (getchar() != '\n');
    }

    printf("\n--- Dados dos Territorios Cadastrados ---\n");

    // Laço de exibição de dados.
    // Percorre o vetor e imprime os dados de cada território em um formato claro.

    for(i = 0; i < 5; i++){
        printf("\n--- Território %d ---\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
    }

    return 0;
    
}