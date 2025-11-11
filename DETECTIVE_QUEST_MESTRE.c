#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAMANHO_HASH 26  // Uma posição para cada letra do alfabeto

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

// Estrutura para a tabela hash (associação pista → suspeito)
typedef struct EntradaHash {
    char pista[100];
    char suspeito[50];
    struct EntradaHash *proximo;  // Para encadeamento em caso de colisão
} EntradaHash;

// Estrutura para contar suspeitos
typedef struct ContadorSuspeito {
    char nome[50];
    int contagem;
    struct ContadorSuspeito *proximo;
} ContadorSuspeito;

// Tabela Hash global
EntradaHash* tabelaHash[TAMANHO_HASH];
ContadorSuspeito* listaSuspeitos = NULL;

// Função hash simples baseada na primeira letra da pista
int funcaoHash(const char* pista) {
    if (pista == NULL || strlen(pista) == 0) {
        return 0;
    }
    char primeiraLetra = tolower(pista[0]);
    return (primeiraLetra - 'a') % TAMANHO_HASH;
}

// Função para inicializar a tabela hash
void inicializarTabelaHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

// Função para inserir associação pista → suspeito na tabela hash
void inserirNaHash(const char* pista, const char* suspeito) {
    if (pista == NULL || strlen(pista) == 0) {
        return;
    }
    
    int indice = funcaoHash(pista);
    
    // Criar nova entrada
    EntradaHash* novaEntrada = (EntradaHash*)malloc(sizeof(EntradaHash));
    if (novaEntrada == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }
    strcpy(novaEntrada->pista, pista);
    strcpy(novaEntrada->suspeito, suspeito);
    novaEntrada->proximo = NULL;
    
    // Inserir na tabela hash (encadeamento para colisões)
    if (tabelaHash[indice] == NULL) {
        tabelaHash[indice] = novaEntrada;
    } else {
        // Adicionar no início da lista encadeada
        novaEntrada->proximo = tabelaHash[indice];
        tabelaHash[indice] = novaEntrada;
    }
    
    printf("✅ Associação registrada: \"%s\" → %s\n", pista, suspeito);
}

// Função para buscar suspeito associado a uma pista
char* buscarSuspeito(const char* pista) {
    if (pista == NULL || strlen(pista) == 0) {
        return "Nenhum suspeito associado";
    }
    
    int indice = funcaoHash(pista);
    EntradaHash* atual = tabelaHash[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    
    return "Nenhum suspeito associado";
}

// Função para atualizar contagem de suspeitos
void atualizarContadorSuspeito(const char* suspeito) {
    if (strcmp(suspeito, "Nenhum suspeito associado") == 0) {
        return;
    }
    
    ContadorSuspeito* atual = listaSuspeitos;
    ContadorSuspeito* anterior = NULL;
    
    // Buscar suspeito na lista
    while (atual != NULL) {
        if (strcmp(atual->nome, suspeito) == 0) {
            atual->contagem++;
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    
    // Suspeito não encontrado, criar novo
    ContadorSuspeito* novo = (ContadorSuspeito*)malloc(sizeof(ContadorSuspeito));
    if (novo == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }
    strcpy(novo->nome, suspeito);
    novo->contagem = 1;
    novo->proximo = NULL;
    
    if (listaSuspeitos == NULL) {
        listaSuspeitos = novo;
    } else {
        anterior->proximo = novo;
    }
}

// Função para encontrar o suspeito mais citado
void encontrarSuspeitoMaisCitado() {
    if (listaSuspeitos == NULL) {
        printf("Nenhuma pista coletada para análise.\n");
        return;
    }
    
    ContadorSuspeito* atual = listaSuspeitos;
    ContadorSuspeito* maisCitado = listaSuspeitos;
    
    while (atual != NULL) {
        if (atual->contagem > maisCitado->contagem) {
            maisCitado = atual;
        }
        atual = atual->proximo;
    }
    
    printf("\n🔎 === SUSPEITO MAIS CITADO ===\n");
    printf("Nome: %s\n", maisCitado->nome);
    printf("Menções: %d pistas associadas\n", maisCitado->contagem);
    printf("===============================\n");
}

// Função para exibir todas as associações pista → suspeito
void exibirAssociacoesPistasSuspeitos() {
    printf("\n📋 === ASSOCIAÇÕES PISTA → SUSPEITO ===\n");
    int totalAssociacoes = 0;
    
    for (int i = 0; i < TAMANHO_HASH; i++) {
        EntradaHash* atual = tabelaHash[i];
        while (atual != NULL) {
            printf("🔍 \"%s\" → %s\n", atual->pista, atual->suspeito);
            totalAssociacoes++;
            atual = atual->proximo;
        }
    }
    
    if (totalAssociacoes == 0) {
        printf("Nenhuma associação registrada.\n");
    } else {
        printf("Total de associações: %d\n", totalAssociacoes);
    }
    printf("=====================================\n");
}

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
    
    return raiz;
}

// Função para buscar uma pista na BST
int buscarPista(Pista* raiz, const char* descricao) {
    if (raiz == NULL) {
        return 0;
    }
    
    int comparacao = strcmp(descricao, raiz->descricao);
    
    if (comparacao == 0) {
        return 1;
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
        char* suspeito = buscarSuspeito(raiz->descricao);
        printf("🔍 %s → Suspeito: %s\n", raiz->descricao, suspeito);
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
    
    printf("🔍 Bem-vindo ao Detective Quest - Nível Mestre!\n");
    printf("Explore a mansão usando as opções:\n");
    printf("'e' - ir para esquerda | 'd' - ir para direita\n");
    printf("'p' - ver pistas e suspeitos | 'a' - ver associações\n");
    printf("'s' - sair e resolver caso\n\n");
    
    while (salaAtual != NULL) {
        printf("\n=========================================\n");
        printf("Você está no: %s\n", salaAtual->nome);
        
        // Verifica se há pista nesta sala e ainda não foi coletada
        if (strlen(salaAtual->pista) > 0 && 
            buscarPista(*arvorePistas, salaAtual->pista) == 0) {
            printf("🎯 Você encontrou uma pista: \"%s\"\n", salaAtual->pista);
            *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);
            
            // Buscar suspeito associado e atualizar contador
            char* suspeito = buscarSuspeito(salaAtual->pista);
            atualizarContadorSuspeito(suspeito);
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
        printf("\nOutras opções: [p] Ver Pistas | [a] Associações | [s] Sair\n");
        
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
                printf("\n📓 === CADERNO DE PISTAS E SUSPEITOS ===\n");
                if (*arvorePistas == NULL) {
                    printf("Nenhuma pista coletada ainda.\n");
                } else {
                    printf("Pistas coletadas (%d no total):\n", contarPistas(*arvorePistas));
                    exibirPistasEmOrdem(*arvorePistas);
                }
                printf("======================================\n");
                break;
            case 'a':
                exibirAssociacoesPistasSuspeitos();
                break;
            case 's':
                printf("👋 Saindo da exploração...\n");
                return;
            default:
                printf("❌ Opção inválida! Use 'e', 'd', 'p', 'a' ou 's'.\n");
        }
    }
}

// Função para inicializar as associações pista → suspeito
void inicializarAssociacoes() {
    // Associar cada pista a um suspeito
    inserirNaHash("Porta principal arrombada", "João");
    inserirNaHash("Copo de vinho meio cheio", "Maria");
    inserirNaHash("Livro sobre venenos aberto", "Dr. Silva");
    inserirNaHash("Documento importante faltando", "Carlos");
    inserirNaHash("Faca desaparecida do bloco", "Ana");
    inserirNaHash("Cadeira desarrumada", "Pedro");
    inserirNaHash("Pegadas de barro", "João");
    inserirNaHash("Retrato riscado", "Maria");
    inserirNaHash("Cheiro estranho", "Dr. Silva");
    inserirNaHash("Cinzas de cigarro raro", "Carlos");
}

// Função principal
int main() {
    // Inicializar tabela hash
    inicializarTabelaHash();
    
    // Configurar associações pista → suspeito
    inicializarAssociacoes();
    
    // Construção da árvore binária representando o mapa da mansão
    Sala* hallEntrada = criarSala("Hall de Entrada", "Porta principal arrombada");
    Sala* salaEstar = criarSala("Sala de Estar", "Copo de vinho meio cheio");
    Sala* corredorPrincipal = criarSala("Corredor Principal", "");
    Sala* biblioteca = criarSala("Biblioteca", "Livro sobre venenos aberto");
    Sala* escritorio = criarSala("Escritório", "Documento importante faltando");
    Sala* cozinha = criarSala("Cozinha", "Faca desaparecida do bloco");
    Sala* salaJantar = criarSala("Sala de Jantar", "Cadeira desarrumada");
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
    printf("\n🎯 === RELATÓRIO FINAL - RESOLUÇÃO DO CASO ===\n");
    printf("Total de pistas coletadas: %d\n", contarPistas(arvorePistas));
    
    if (arvorePistas != NULL) {
        printf("\nTodas as pistas e suspeitos associados:\n");
        exibirPistasEmOrdem(arvorePistas);
    }
    
    printf("\n📊 Análise final dos suspeitos:\n");
    exibirAssociacoesPistasSuspeitos();
    encontrarSuspeitoMaisCitado();
    
    printf("\n💡 Baseado nas evidências, o suspeito mais citado é o principal candidato a culpado!\n");
    printf("==================================================\n");
    
    return 0;
}