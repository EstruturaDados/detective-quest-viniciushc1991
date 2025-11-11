#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma sala (nó da árvore binária do mapa)
typedef struct Sala {
    char nome[50];
    char pista[100];  // Pista encontrada nesta sala (vazia se não tiver pista)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para a árvore de busca de pistas
typedef struct Pista {
    char descricao[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// Função para criar uma nova sala
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para inserir uma pista na BST (ordem alfabética)
Pista* inserirPista(Pista* raiz, const char* descricao) {
    if (raiz == NULL) {
        Pista* novaPista = (Pista*)malloc(sizeof(Pista));
        if (novaPista == NULL) {
            printf("Erro de alocação de memória!\n");
            exit(1);
        }
        strcpy(novaPista->descricao, descricao);
        novaPista->esquerda = NULL;
        novaPista->direita = NULL;
        return novaPista;
    }
    
    int comparacao = strcmp(descricao, raiz->descricao);
    
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, descricao);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, descricao);
    }
    // Se for igual, não insere duplicata
    
    return raiz;
}

// Função para buscar uma pista na BST
int buscarPista(Pista* raiz, const char* descricao) {
    if (raiz == NULL) {
        return 0; // Não encontrada
    }
    
    int comparacao = strcmp(descricao, raiz->descricao);
    
    if (comparacao == 0) {
        return 1; // Encontrada
    } else if (comparacao < 0) {
        return buscarPista(raiz->esquerda, descricao);
    } else {
        return buscarPista(raiz->direita, descricao);
    }
}

// Função para exibir todas as pistas em ordem alfabética
void exibirPistasEmOrdem(Pista* raiz) {
    if (raiz != NULL) {
        exibirPistasEmOrdem(raiz->esquerda);
        printf("🔍 %s\n", raiz->descricao);
        exibirPistasEmOrdem(raiz->direita);
    }
}

// Função para contar o total de pistas coletadas
int contarPistas(Pista* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return 1 + contarPistas(raiz->esquerda) + contarPistas(raiz->direita);
}

// Função para explorar as salas interativamente
void explorarSalas(Sala* raiz, Pista** arvorePistas) {
    Sala* salaAtual = raiz;
    char opcao;
    
    printf("🔍 Bem-vindo ao Detective Quest - Nível Aventureiro!\n");
    printf("Explore a mansão usando as opções:\n");
    printf("'e' - ir para esquerda | 'd' - ir para direita\n");
    printf("'p' - ver pistas coletadas | 's' - sair\n\n");
    
    while (salaAtual != NULL) {
        printf("\n=========================================\n");
        printf("Você está no: %s\n", salaAtual->nome);
        
        // Verifica se há pista nesta sala e ainda não foi coletada
        if (strlen(salaAtual->pista) > 0 && 
            buscarPista(*arvorePistas, salaAtual->pista) == 0) {
            printf("🎯 Você encontrou uma pista: \"%s\"\n", salaAtual->pista);
            *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);
            printf("✅ Pista adicionada ao seu caderno!\n");
        }
        
        // Verifica se é um nó folha
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("🏁 Fim do caminho! Esta sala não tem saídas.\n");
            break;
        }
        
        // Mostra opções disponíveis
        printf("\nOpções de navegação: ");
        if (salaAtual->esquerda != NULL) printf("[e] Esquerda ");
        if (salaAtual->direita != NULL) printf("[d] Direita ");
        printf("\nOutras opções: [p] Ver Pistas | [s] Sair\n");
        
        printf("\nPara onde deseja ir? ");
        scanf(" %c", &opcao);
        
        switch (opcao) {
            case 'e':
                if (salaAtual->esquerda != NULL) {
                    salaAtual = salaAtual->esquerda;
                } else {
                    printf("❌ Não há sala à esquerda!\n");
                }
                break;
            case 'd':
                if (salaAtual->direita != NULL) {
                    salaAtual = salaAtual->direita;
                } else {
                    printf("❌ Não há sala à direita!\n");
                }
                break;
            case 'p':
                printf("\n📓 === CADERNO DE PISTAS ===\n");
                if (*arvorePistas == NULL) {
                    printf("Nenhuma pista coletada ainda.\n");
                } else {
                    printf("Pistas coletadas (%d no total):\n", contarPistas(*arvorePistas));
                    exibirPistasEmOrdem(*arvorePistas);
                }
                printf("=============================\n");
                break;
            case 's':
                printf("👋 Saindo da exploração...\n");
                return;
            default:
                printf("❌ Opção inválida! Use 'e', 'd', 'p' ou 's'.\n");
        }
    }
}

// Função principal que constrói a árvore automaticamente
int main() {
    // Construção da árvore binária representando o mapa da mansão
    // Agora com pistas em algumas salas
    Sala* hallEntrada = criarSala("Hall de Entrada", "Porta principal arrombada");
    
    // Primeiro nível
    Sala* salaEstar = criarSala("Sala de Estar", "Copo de vinho meio cheio");
    Sala* corredorPrincipal = criarSala("Corredor Principal", "");
    
    // Segundo nível - esquerda
    Sala* biblioteca = criarSala("Biblioteca", "Livro sobre venenos aberto");
    Sala* escritorio = criarSala("Escritório", "Documento importante faltando");
    
    // Segundo nível - direita  
    Sala* cozinha = criarSala("Cozinha", "Faca desaparecida do bloco");
    Sala* salaJantar = criarSala("Sala de Jantar", "Cadeira desarrumada");
    
    // Terceiro nível - folhas
    Sala* jardimInverno = criarSala("Jardim de Inverno", "Pegadas de barro");
    Sala* salaoBailes = criarSala("Salão de Bailes", "Retrato riscado");
    Sala* despensa = criarSala("Despensa", "Cheiro estranho");
    Sala* varanda = criarSala("Varanda", "Cinzas de cigarro raro");
    
    // Montagem da árvore
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = corredorPrincipal;
    
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = escritorio;
    
    corredorPrincipal->esquerda = cozinha;
    corredorPrincipal->direita = salaJantar;
    
    biblioteca->esquerda = jardimInverno;
    biblioteca->direita = salaoBailes;
    
    cozinha->esquerda = despensa;
    salaJantar->direita = varanda;
    
    // Árvore de pistas (inicialmente vazia)
    Pista* arvorePistas = NULL;
    
    // Iniciar exploração interativa
    explorarSalas(hallEntrada, &arvorePistas);
    
    // Relatório final
    printf("\n🎯 === EXPLORAÇÃO CONCLUÍDA ===\n");
    printf("Total de pistas coletadas: %d\n", contarPistas(arvorePistas));
    if (arvorePistas != NULL) {
        printf("\nPistas em ordem alfabética:\n");
        exibirPistasEmOrdem(arvorePistas);
    }
    printf("===============================\n");
    
    // Em um programa real, seria necessário implementar funções
    // para liberar a memória de ambas as árvores
    
    return 0;
}