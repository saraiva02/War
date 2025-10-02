#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Cores disponíveis para os jogadores
char* CORES_DISPONIVEIS[] = {"Azul", "Vermelho", "Verde", "Amarelo", "Roxo", "Ciano"};
#define TOTAL_CORES (sizeof(CORES_DISPONIVEIS) / sizeof(CORES_DISPONIVEIS[0]))

// Definição da struct Territorio
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Definição da struct Jogador
typedef struct {
    char cor[10];
    char* missao; // Ponteiro para a missão alocada dinamicamente
} Jogador;

// Protótipos das funções
void cadastrarTerritorios(Territorio* mapa, int numTerritorios, int numJogadores, int tropasDeReforco); 
void exibirTerritorios(Territorio* mapa, int numTerritorios); 
void exibirMenu(Jogador* jogador); 
int atacar(Territorio* atacante, Territorio* defensor); 
// ATUALIZADO: Recebe a lista de missões usadas para garantir exclusividade.
void atribuirMissao(Jogador* jogador, char* missoes[], int totalMissoes, char** missoesUsadas, int numJogadores); 
void exibirMissao(char* missao);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador); 
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int numJogadores);


// MISSOES (10 OPÇÕES)
char* MISSOES[] = {
    "Conquistar no minimo 5 territorios no total.",
    "Eliminar todas as tropas da cor Vermelho.",
    "Ter pelo menos 3 territorios e 15 tropas no total.",
    "Eliminar todas as tropas da cor Azul.",
    "Eliminar todas as tropas da cor Verde.",
    "Possuir 10 ou mais tropas em um unico territorio.",
    "Conquistar no minimo 4 territorios no total.",
    "Possuir um total de 25 ou mais tropas no final do turno.",
    "Eliminar todas as tropas da cor Amarelo.",
    "Ter um territorio que comece com 'A' e possuir no minimo 5 tropas nele."
};
#define TOTAL_MISSOES (sizeof(MISSOES) / sizeof(MISSOES[0]))
#define MIN_TERRITORIOS 5 
#define TROPAS_REFORCO_INICIAL 10 

int main() {
    int numTerritorios;
    int numJogadores;
    Territorio* mapa;
    Jogador* jogadores;
    int opcao = 1; 
    
    srand(time(NULL));

    printf("--- Simulacao de Batalhas e Missoes Estrategicas ---\n");
    
    // Leitura do número de jogadores
    do {
        printf("Digite o numero de jogadores (max %d): ", TOTAL_CORES);
        if (scanf("%d", &numJogadores) != 1 || numJogadores <= 0 || numJogadores > TOTAL_CORES) {
            printf("[ERRO] Numero de jogadores invalido. Tente novamente.\n");
            numJogadores = -1; 
        }
        while (getchar() != '\n');
    } while (numJogadores <= 0);


    // Leitura do número de territórios
    do {
        printf("Digite o numero de territorios que deseja cadastrar (min %d): ", MIN_TERRITORIOS);
        if (scanf("%d", &numTerritorios) != 1 || numTerritorios < MIN_TERRITORIOS) {
            printf("[ERRO] Numero de territorios invalido. Deve ser pelo menos igual a %d para cumprir as missoes. Tente novamente.\n", MIN_TERRITORIOS);
            numTerritorios = -1; 
        }
        while (getchar() != '\n');
    } while (numTerritorios < MIN_TERRITORIOS);

    // Alocação de memória
    mapa = (Territorio*) calloc(numTerritorios, sizeof(Territorio));
    if (mapa == NULL) { printf("Erro ao alocar memoria para territorios.\n"); return 1; }
    
    jogadores = (Jogador*) calloc(numJogadores, sizeof(Jogador));
    if (jogadores == NULL) { 
        printf("Erro ao alocar memoria para jogadores.\n"); 
        free(mapa);
        return 1; 
    }
    
    // --- INICIALIZAÇÃO DE JOGADORES E MISSÕES ---
    system("cls || clear");
    printf("--- JOGADORES E MISSOES ATRIBUIDAS ---\n");
    
    // Array para rastrear quais missões foram usadas
    char** missoesUsadas = (char**) calloc(numJogadores, sizeof(char*));
    if (missoesUsadas == NULL) {
        printf("Erro ao alocar memoria para rastrear missoes.\n");
        liberarMemoria(mapa, jogadores, numJogadores); 
        return 1;
    }
    
    size_t maxLen = 0;
    for (int m = 0; m < TOTAL_MISSOES; m++) {
        if (strlen(MISSOES[m]) > maxLen) maxLen = strlen(MISSOES[m]);
    }

    // Atribuição de missões
    for (int j = 0; j < numJogadores; j++) {
        strcpy(jogadores[j].cor, CORES_DISPONIVEIS[j]);
        
        jogadores[j].missao = (char*) malloc(maxLen + 1);
        if (jogadores[j].missao == NULL) {
             printf("Erro ao alocar memoria para missao.\n");
             liberarMemoria(mapa, jogadores, j);
             free(missoesUsadas);
             return 1;
        }

        // ATUALIZADO: Chama a função que garante exclusividade e validade
        atribuirMissao(&jogadores[j], MISSOES, TOTAL_MISSOES, missoesUsadas, j);
        
        printf("JOGADOR %d - Cor: %s | Missao: ", j + 1, jogadores[j].cor);
        exibirMissao(jogadores[j].missao);
        
        // Adiciona a missão sorteada à lista de usadas
        missoesUsadas[j] = jogadores[j].missao;
    }
    
    printf("\nPressione ENTER para iniciar o cadastro e distribuicao automatica de tropas...");
    getchar();

    // Cadastro dos territórios e distribuição de tropas de reforço
    cadastrarTerritorios(mapa, numTerritorios, numJogadores, TROPAS_REFORCO_INICIAL); 
    
    // ------------------------------------------------------------------
    // Início do Loop de Turnos (Lógica mantida)
    // ------------------------------------------------------------------
    while (opcao != 0) {
        system("cls || clear");
        
        exibirTerritorios(mapa, numTerritorios);
        printf("================================================================\n"); 
        
        exibirMenu(&jogadores[0]); 

        if (scanf("%d", &opcao) != 1) { opcao = -1; }
        while (getchar() != '\n'); 

        if (opcao == 0) { printf("\nSaindo do jogo...\n"); break; } 
        
        if (opcao == 2) {
             system("cls || clear");
             if (verificarMissao(jogadores[0].missao, mapa, numTerritorios, jogadores[0].cor)) {
                printf("\n*** PARABENS! Sua missao ja foi cumprida! Vitoria! ***\n");
                opcao = 0; 
             } else {
                printf("\n--- Missao ainda nao cumprida. Continue atacando! ---\n");
             }
             printf("\nPressione ENTER para retornar ao menu...");
             getchar();
             continue; 
        }
        
        if (opcao != 1) { printf("\nOpcao invalida. Pressione ENTER para continuar..."); getchar(); continue; }

        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d): ", numTerritorios);
        int indiceAtacante;
        if (scanf("%d", &indiceAtacante) != 1) { while (getchar() != '\n'); continue; }
        while (getchar() != '\n');
        
        printf("Escolha o territorio defensor (1 a %d): ", numTerritorios);
        int indiceDefensor;
        if (scanf("%d", &indiceDefensor) != 1) { while (getchar() != '\n'); continue; }
        while (getchar() != '\n');
        
        int idxAtaque = indiceAtacante - 1;
        int idxDefesa = indiceDefensor - 1;

        if (idxAtaque >= 0 && idxAtaque < numTerritorios &&
            idxDefesa >= 0 && idxDefesa < numTerritorios &&
            strcmp(mapa[idxAtaque].cor, mapa[idxDefesa].cor) != 0 &&
            mapa[idxAtaque].tropas > 1) 
        {
            printf("\n--- INICIO DO ATAQUE ---\n");
            atacar(&mapa[idxAtaque], &mapa[idxDefesa]);
            printf("\n--- RESULTADO DO TURNO ---\n");
            exibirTerritorios(mapa, numTerritorios); 
        } else {
            printf("\nAtaque invalido! Verifique indices, cores e tropas (minimo 2).\n");
        }

        printf("\nPressione ENTER para continuar para o proximo turno...");
        getchar();
        
        if (verificarMissao(jogadores[0].missao, mapa, numTerritorios, jogadores[0].cor)) {
            printf("\n\n*** VITORIA! O jogador %s cumpriu sua missao e venceu o jogo! ***\n", jogadores[0].cor);
            opcao = 0; 
        }
    }
    // Fim do Loop de Turnos

    // Libera toda a memória alocada
    liberarMemoria(mapa, jogadores, numJogadores);
    free(missoesUsadas);

    return 0;
}

// -----------------------------------------------------------------------------
// FUNÇÃO ATRIBUIR MISSAO (MODIFICADA)
// -----------------------------------------------------------------------------

void atribuirMissao(Jogador* jogador, char* missoes[], int totalMissoes, char** missoesUsadas, int numJogadoresAtuais) {
    int indiceSorteado;
    char* missaoCandidata;
    int repetida;
    int invalida;

    // Loop que garante que a missão seja ÚNICA e VÁLIDA (não elimine a própria cor)
    do {
        repetida = 0;
        invalida = 0;
        
        indiceSorteado = rand() % totalMissoes;
        missaoCandidata = missoes[indiceSorteado];

        // 1. Checa por repetição
        for (int i = 0; i < numJogadoresAtuais; i++) {
            if (strcmp(missaoCandidata, missoesUsadas[i]) == 0) {
                repetida = 1;
                break;
            }
        }

        // 2. Checa por validade (Eliminar a própria cor)
        if (strstr(missaoCandidata, "Eliminar todas as tropas da cor") != NULL) {
            if (strstr(missaoCandidata, jogador->cor) != NULL) {
                invalida = 1;
            }
        }
        
    } while (repetida || invalida);
    
    strcpy(jogador->missao, missaoCandidata);
}

// -----------------------------------------------------------------------------
// OUTRAS FUNÇÕES (Lógica mantida)
// -----------------------------------------------------------------------------

void cadastrarTerritorios(Territorio* mapa, int numTerritorios, int numJogadores, int tropasDeReforco) {
    char nomeTerritorio[30];
    
    for (int i = 0; i < numTerritorios; i++) {
        
        system("cls || clear");
        printf("--- Cadastro de Territorios ---\n");
        printf("\nTerritorio %d:\n", i + 1);
        printf("Nome do Territorio: ");
        fgets(nomeTerritorio, 30, stdin);
        nomeTerritorio[strcspn(nomeTerritorio, "\n")] = 0; 
        strcpy(mapa[i].nome, nomeTerritorio);

        char* corAtribuida = CORES_DISPONIVEIS[i % numJogadores];
        strcpy(mapa[i].cor, corAtribuida);
        
        mapa[i].tropas = 1; 
    }

    for (int i = 0; i < tropasDeReforco; i++) {
        int indiceSorteado = rand() % numTerritorios;
        mapa[indiceSorteado].tropas++;
    }

    system("cls || clear");
    printf("\n--- DISTRIBUICAO INICIAL COMPLETA ---\n");
    printf("Territorios divididos e %d tropas de reforco distribuidas aleatoriamente.\n", tropasDeReforco);
    printf("\nPressione ENTER para iniciar o jogo...");
    getchar();
}

void exibirTerritorios(Territorio* mapa, int numTerritorios) {
    printf("\n--- TERRITORIOS ATUAIS ---\n");
    for (int i = 0; i < numTerritorios; i++) {
        printf("%d. %s (Exercito: %s , Tropas: %d)\n", 
               i + 1, 
               mapa[i].nome, 
               mapa[i].cor, 
               mapa[i].tropas);
    }
}

void exibirMenu(Jogador* jogador) {
    printf("\n--- SUA MISSAO (Exercito %s) ---\n", jogador->cor);
    printf("%s\n", jogador->missao);
    printf("================================================================\n"); 
    printf("--- MENU DE ACOES ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");
}

void exibirMissao(char* missao) {
    printf("%s\n", missao);
}

int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    int countTerritorios = 0;
    int countTropas = 0;
    
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            countTerritorios++;
            countTropas += mapa[i].tropas;
        }
    }
    
    // Lógica 1: "Conquistar no minimo 5 territorios no total."
    if (strstr(missao, "Conquistar no minimo 5 territorios no total") != NULL) {
        if (countTerritorios >= 5) return 1;
    }

    // Lógica 2: "Ter pelo menos 3 territorios e 15 tropas no total."
    if (strstr(missao, "Ter pelo menos 3 territorios e 15 tropas no total") != NULL) {
        if (countTerritorios >= 3 && countTropas >= 15) return 1;
    }

    // Lógicas de Eliminação (Vermelho, Azul, Verde, Amarelo)
    if (strstr(missao, "Eliminar todas as tropas da cor") != NULL) {
        char corAlvo[10];
        if (strstr(missao, "Vermelho") != NULL) strcpy(corAlvo, "Vermelho");
        else if (strstr(missao, "Azul") != NULL) strcpy(corAlvo, "Azul");
        else if (strstr(missao, "Verde") != NULL) strcpy(corAlvo, "Verde");
        else if (strstr(missao, "Amarelo") != NULL) strcpy(corAlvo, "Amarelo");
        else return 0; 

        if (strcmp(corJogador, corAlvo) != 0) { 
            int tropasAlvo = 0;
            for (int i = 0; i < tamanho; i++) {
                if (strcmp(mapa[i].cor, corAlvo) == 0) {
                    tropasAlvo += mapa[i].tropas;
                }
            }
            if (tropasAlvo == 0) return 1; 
        }
    }

    // Lógica 6: "Possuir 10 ou mais tropas em um unico territorio."
    if (strstr(missao, "Possuir 10 ou mais tropas em um unico territorio") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 10) {
                return 1; 
            }
        }
    }

    // Lógica 7: "Conquistar no minimo 4 territorios no total."
    if (strstr(missao, "Conquistar no minimo 4 territorios no total") != NULL) {
        if (countTerritorios >= 4) return 1;
    }

    // Lógica 8: "Possuir um total de 25 ou mais tropas no final do turno."
    if (strstr(missao, "Possuir um total de 25 ou mais tropas no final do turno") != NULL) {
        if (countTropas >= 25) return 1;
    }
    
    // Lógica 10: "Ter um territorio que comece com 'A' e possuir no minimo 5 tropas nele."
    if (strstr(missao, "Ter um territorio que comece com 'A'") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && 
                (mapa[i].nome[0] == 'A' || mapa[i].nome[0] == 'a') &&
                mapa[i].tropas >= 5) 
            {
                return 1;
            }
        }
    }
    
    return 0;
}


int atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;
    
    printf("\nBatalha entre %s (Atacante) e %s (Defensor)!\n", atacante->nome, defensor->nome);
    printf("Dado do Atacante: %d\n", dadoAtacante);
    printf("Dado do Defensor: %d\n", dadoDefensor);
    
    if (dadoAtacante > dadoDefensor) {
        printf("O atacante venceu a batalha!\n");
        if (defensor->tropas == 1) { 
             printf("\n*** CONQUISTA REALIZADA! ***\n");
             strcpy(defensor->cor, atacante->cor);
             int tropasTransferidas = atacante->tropas - 1;
             defensor->tropas = tropasTransferidas; 
             atacante->tropas = 1; 
             printf("%s foi CONQUISTADO e agora pertence a cor %s com %d tropas.\n", defensor->nome, defensor->cor, defensor->tropas);
             printf("%s agora tem 1 tropa restante (minimo).\n", atacante->nome);
        } else { 
             printf("Defensor perdeu 1 tropa.\n");
             defensor->tropas--;
             if (atacante->tropas > 1) { 
                atacante->tropas--;
                defensor->tropas++;
                printf("%s moveu 1 tropa e tem %d tropas restantes.\n", atacante->nome, atacante->tropas);
             }
             printf("%s tem %d tropas restantes.\n", defensor->nome, defensor->tropas);
        }
    } else {
        if (atacante->tropas > 1) { 
            atacante->tropas--;
            printf("O defensor venceu a batalha! %s perdeu uma tropa.\n", atacante->nome);
        } else {
            printf("O defensor venceu a batalha! Nao houve perda de tropas.\n");
        }
    }
    return 1;
}

void liberarMemoria(Territorio* mapa, Jogador* jogadores, int numJogadores) {
    for (int j = 0; j < numJogadores; j++) {
        if (jogadores[j].missao) {
            free(jogadores[j].missao);
        }
    }
    free(jogadores);
    free(mapa);
    printf("\nMemoria liberada com sucesso.\n");
}