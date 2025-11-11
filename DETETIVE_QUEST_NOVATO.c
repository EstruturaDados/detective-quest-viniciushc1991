#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma sala (nó da árvore binária)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Função para criar uma nova sala
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para explorar as salas interativamente
void explorarSalas(Sala* raiz) {
    Sala* salaAtual = raiz;
    char opcao;
    
    printf("🔍 Bem-vindo ao Detective Quest - Nível Novato!\n");
    printf("Explore a mansão usando as opções:\n");
    printf("'e' - ir para esquerda | 'd' - ir para direita | 's' - sair\n\n");
    
    while (salaAtual != NULL) {
        printf("Você está no: %s\n", salaAtual->nome);
        
        // Verifica se é um nó folha
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("🏁 Fim do caminho! Esta sala não tem saídas.\n");
            break;
        }
        
        // Mostra opções disponíveis
        printf("Opções disponíveis: ");
        if (salaAtual->esquerda != NULL) printf("[e] Esquerda ");
        if (salaAtual->direita != NULL) printf("[d] Direita ");
        printf("[s] Sair\n");
        
        printf("Para onde deseja ir? ");
        scanf(" %c", &opcao);
        
        switch (opcao) {
            case 'e':
                if (salaAtual->esquerda != NULL) {
                    salaAtual = salaAtual->esquerda;
                } else {
                    printf("❌ Não há sala à esquerda!\n\n");
                }
                break;
            case 'd':
                if (salaAtual->direita != NULL) {
                    salaAtual = salaAtual->direita;
                } else {
                    printf("❌ Não há sala à direita!\n\n");
                }
                break;
            case 's':
                printf("👋 Saindo da exploração...\n");
                return;
            default:
                printf("❌ Opção inválida! Use 'e', 'd' ou 's'.\n\n");
        }
    }
}

// Função principal que constrói a árvore automaticamente
int main() {
    // Construção da árvore binária representando o mapa da mansão
    Sala* hallEntrada = criarSala("Hall de Entrada");
    
    // Primeiro nível
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* corredorPrincipal = criarSala("Corredor Principal");
    
    // Segundo nível - esquerda
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* escritorio = criarSala("Escritório");
    
    // Segundo nível - direita  
    Sala* cozinha = criarSala("Cozinha");
    Sala* salaJantar = criarSala("Sala de Jantar");
    
    // Terceiro nível - folhas
    Sala* jardimInverno = criarSala("Jardim de Inverno");
    Sala* salaoBailes = criarSala("Salão de Bailes");
    Sala* despensa = criarSala("Despensa");
    Sala* varanda = criarSala("Varanda");
    
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
    
    // Iniciar exploração interativa
    explorarSalas(hallEntrada);
    
    printf("\n🎯 Exploração concluída!\n");
    
    // Liberar memória (opcional para este exercício)
    // Em um programa real, seria necessário implementar uma função
    // para liberar toda a árvore recursivamente
    
    return 0;
}