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
int atacar(Territorio* atacante, Territorio* defensor); 
void liberarMemoria(Territorio* mapa);

int main() {
    int numTerritorios;
    Territorio* mapa;
    int indiceAtacante, indiceDefensor;
    int opcao = 1; // Variável de controle para o loop de turnos
    
    // Utilizar srand(time(NULL)) para garantir aleatoriedade
    srand(time(NULL));

    // Alocação dinâmica de territórios: pede ao usuário o número total de territórios
    printf("--- Simulacao de Batalhas entre Territorios ---\n");
    printf("Digite o numero de territorios que deseja cadastrar: ");
    if (scanf("%d", &numTerritorios) != 1 || numTerritorios <= 0) {
        printf("Numero de territorios invalido.\n");
        return 1;
    }
    while (getchar() != '\n'); // Limpa o buffer de entrada

    // Utiliza calloc para alocar memória para o vetor de struct
    mapa = (Territorio*) calloc(numTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    // Cadastro dos territórios
    cadastrarTerritorios(mapa, numTerritorios);
    
    // ------------------------------------------------------------------
    // Início do Loop de Turnos
    // ------------------------------------------------------------------
    while (opcao != 0) {
        system("cls || clear");
        printf("--- FASE DE ATAQUE (Turno Atual) ---\n");
        exibirTerritorios(mapa, numTerritorios);

        printf("\nOpcoes:\n");
        printf("  [1] Realizar um ataque\n");
        printf("  [0] Sair do jogo\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1; // Valor para garantir a limpeza de buffer e loop
        }
        while (getchar() != '\n'); // Limpa o buffer de entrada

        if (opcao == 0) {
            printf("\nSaindo do jogo...\n");
            break;
        } else if (opcao != 1) {
            printf("\nOpcao invalida. Tente novamente.\n");
            // Pequena pausa para o usuário ver a mensagem de erro
            printf("Pressione ENTER para continuar...");
            getchar();
            continue;
        }

        // Simulação de ataques
        printf("\nEscolha o territorio atacante (1 a %d): ", numTerritorios);
        if (scanf("%d", &indiceAtacante) != 1) {
            printf("Entrada invalida. Voltando ao menu.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        
        printf("Escolha o territorio defensor (1 a %d): ", numTerritorios);
        if (scanf("%d", &indiceDefensor) != 1) {
            printf("Entrada invalida. Voltando ao menu.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        
        // Ajustar índices para 0-based
        int idxAtaque = indiceAtacante - 1;
        int idxDefesa = indiceDefensor - 1;

        // Validar as escolhas
        if (idxAtaque >= 0 && idxAtaque < numTerritorios &&
            idxDefesa >= 0 && idxDefesa < numTerritorios &&
            strcmp(mapa[idxAtaque].cor, mapa[idxDefesa].cor) != 0 &&
            mapa[idxAtaque].tropas > 1) // Atacante deve ter pelo menos 2 tropas para atacar (1 fica no território)
        {
            printf("\n--- INICIO DO ATAQUE ---\n");
            atacar(&mapa[idxAtaque], &mapa[idxDefesa]);
            
            printf("\n--- FIM DO TURNO ---\n");
            exibirTerritorios(mapa, numTerritorios);
        } else {
            printf("\nAtaque invalido!\n");
            printf("- Verifique se os indices estao corretos.\n");
            printf("- Territorios nao podem ser da mesma cor.\n");
            printf("- O atacante precisa de mais de 1 tropa para atacar.\n");
        }

        // Pequena pausa para o usuário ver o resultado do turno
        printf("\nPressione ENTER para continuar para o proximo turno...");
        getchar();
    }
    // ------------------------------------------------------------------
    // Fim do Loop de Turnos
    // ------------------------------------------------------------------

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
        
        printf("Quantidade de tropas (min 1): ");
        if (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 1) {
             mapa[i].tropas = 1; // Garante que a tropa mínima é 1
        }
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

// Funçao de ataque: Agora retorna 1 se a batalha ocorreu, 0 caso contrário (embora a validação já ocorra no main)
int atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;
    
    printf("\nBatalha entre %s (Atacante) e %s (Defensor)!\n", atacante->nome, defensor->nome);
    printf("Dado do Atacante: %d\n", dadoAtacante);
    printf("Dado do Defensor: %d\n", dadoDefensor);
    
    if (dadoAtacante > dadoDefensor) {
        printf("O atacante venceu a batalha!\n");
        
        if (defensor->tropas == 1) {
            // Lógica de Conquista: defensor tinha apenas 1 tropa
            printf("\n*** CONQUISTA REALIZADA! ***\n");
            
            // 1. Defensor perde a cor e passa a ter a cor do atacante
            strcpy(defensor->cor, atacante->cor);
            
            // 2. Transfere todas as tropas do atacante, exceto 1, para o novo território
            // O atacante DEVE deixar no mínimo 1 tropa no território de origem.
            int tropasTransferidas = atacante->tropas - 1;
            
            // 3. A tropa que restava no defensor é substituída pelas tropas de ataque
            defensor->tropas = tropasTransferidas; 
            atacante->tropas = 1; // O atacante fica com 1 tropa no território original
            
            printf("%s foi CONQUISTADO e agora pertence a cor %s com %d tropas.\n", 
                   defensor->nome, defensor->cor, defensor->tropas);
            printf("%s agora tem 1 tropa restante (minimo).\n", atacante->nome);
            
        } else {
            // Lógica de Batalha Padrão: defensor tinha mais de 1 tropa
            printf("Defensor perdeu 1 tropa.\n");
            defensor->tropas--;
            // O atacante pode mover 1 tropa (ou mais, dependendo das regras do jogo original)
            // Mantendo a lógica anterior: se vence, transfere metade (pode ser ajustado)
            // Vou ajustar para uma perda/ganho mais simples em caso de não-conquista:
            
            // Neste caso, o atacante venceu, o defensor perdeu 1 tropa.
            // O atacante move 1 tropa para o território defensor.
            if (atacante->tropas > 1) { // Verifica se há tropas para mover
                 atacante->tropas--;
                 defensor->tropas++;
                 printf("%s moveu 1 tropa e tem %d tropas restantes.\n", atacante->nome, atacante->tropas);
            }
            printf("%s tem %d tropas restantes.\n", defensor->nome, defensor->tropas);
        }

    } else {
        printf("O defensor venceu a batalha!\n");
        // Se perder, o atacante perde uma tropa
        if (atacante->tropas > 1) { // Garante que o atacante fique com pelo menos 1 tropa
            atacante->tropas--;
            printf("%s perdeu uma tropa e agora tem %d tropas.\n", atacante->nome, atacante->tropas);
        } else {
            printf("%s nao perdeu tropas (tropas minimas).\n", atacante->nome);
        }
    }
    return 1;
}

// Liberação de memória: cria uma função para liberar o espaço alocado
void liberarMemoria(Territorio* mapa) {
    free(mapa);
    printf("\nMemoria liberada com sucesso.\n");
}