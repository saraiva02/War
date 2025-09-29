#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struct atualizada: define a estrutura para um território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Protótipos das funções
void cadastrarTerritorios(Territorio* mapa, int numTerritorios);
void exibirTerritorios(Territorio* mapa, int numTerritorios);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

int main() {
    int numTerritorios;
    Territorio* mapa;
    int indiceAtacante, indiceDefensor;
    
    // Utilizar srand(time(NULL)) para garantir aleatoriedade
    srand(time(NULL));

    // Alocação dinâmica de territórios: pede ao usuário o número total de territórios
    printf("--- Simulacao de Batalhas entre Territorios ---\n");
    printf("Digite o numero de territorios que deseja cadastrar: ");
    scanf("%d", &numTerritorios);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    // Utiliza calloc para alocar memória para o vetor de struct
    mapa = (Territorio*) calloc(numTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    // Cadastro dos territórios
    cadastrarTerritorios(mapa, numTerritorios);
    
    // Simulação de ataques
    system("cls || clear");
    printf("--- FASE DE ATAQUE ---\n");
    exibirTerritorios(mapa, numTerritorios);
    
    printf("\nEscolha o territorio atacante (1 a %d): ", numTerritorios);
    scanf("%d", &indiceAtacante);
    while (getchar() != '\n');
    
    printf("Escolha o territorio defensor (1 a %d): ", numTerritorios);
    scanf("%d", &indiceDefensor);
    while (getchar() != '\n');
    
    // Validar as escolhas de ataque para que o jogador não ataque um território da própria cor
    if (indiceAtacante >= 1 && indiceAtacante <= numTerritorios &&
        indiceDefensor >= 1 && indiceDefensor <= numTerritorios &&
        strcmp(mapa[indiceAtacante - 1].cor, mapa[indiceDefensor - 1].cor) != 0) {
        
        atacar(&mapa[indiceAtacante - 1], &mapa[indiceDefensor - 1]);
        
        printf("\n--- RESULTADO DO ATAQUE ---\n");
        exibirTerritorios(mapa, numTerritorios);
    } else {
        printf("Ataque invalido! Verifique os indices ou se os territorios sao da mesma cor.\n");
    }

    // Gerenciamento de memória: toda memória alocada dinamicamente deve ser liberada
    liberarMemoria(mapa);

    return 0;
}

// Funções para modularizar o código
void cadastrarTerritorios(Territorio* mapa, int numTerritorios) {
    int i;
    for (i = 0; i < numTerritorios; i++) {
        system("cls || clear");
        printf("--- Cadastro de Territorios ---\n");
        printf("\nTerritorio %d:\n", i + 1);

        printf("Nome do territorio: ");
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;
        
        printf("Cor do exercito: ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = 0;
        
        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        while (getchar() != '\n');
    }
}

void exibirTerritorios(Territorio* mapa, int numTerritorios) {
    int i;
    printf("\n--- Dados dos Territorios ---\n");
    for (i = 0; i < numTerritorios; i++) {
        printf("\n--- Territorio %d ---\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}

// Funçao de ataque: utiliza rand() para simular um dado de ataque
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;
    
    printf("\nBatalha entre %s (Atacante) e %s (Defensor)!\n", atacante->nome, defensor->nome);
    printf("Dado do Atacante: %d\n", dadoAtacante);
    printf("Dado do Defensor: %d\n", dadoDefensor);
    
    if (dadoAtacante > dadoDefensor) {
        printf("O atacante venceu a batalha!\n");
        // Atualização dos campos: transfere a cor e metade das tropas para o território defensor
        strcpy(defensor->cor, atacante->cor);
        int tropasTransferidas = atacante->tropas / 2;
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
        
        printf("%s agora pertence a cor %s com %d tropas.\n", defensor->nome, defensor->cor, defensor->tropas);
        printf("%s agora tem %d tropas restantes.\n", atacante->nome, atacante->tropas);

    } else {
        printf("O defensor venceu a batalha!\n");
        // Se perder, o atacante perde uma tropa
        if (atacante->tropas > 0) {
            atacante->tropas--;
        }
        printf("%s perdeu uma tropa e agora tem %d tropas.\n", atacante->nome, atacante->tropas);
    }
}

// Liberação de memória: cria uma função para liberar o espaço alocado
void liberarMemoria(Territorio* mapa) {
    free(mapa);
    printf("\nMemoria liberada com sucesso.\n");
}