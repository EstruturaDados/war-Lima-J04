// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        #include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TERRITORIOS 100
#define TOTAL_MISSOES 5
#define MAX_JOGADORES 2

// Struct Territorio
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Protótipos de funções
Territorio* cadastrarTerritorios(int total);
void exibirTerritorios(Territorio* territorios, int total);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* territorios, char** missoes, int totalJogadores);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int total);

// Missões disponíveis
char* missoesDisponiveis[TOTAL_MISSOES] = {
    "Conquistar 3 territórios de cores diferentes",
    "Eliminar todas as tropas da cor vermelha",
    "Controlar pelo menos 4 territórios",
    "Ter um território com mais de 10 tropas",
    "Conquistar dois territórios seguidos na lista"
};
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    int totalTerritorios;
    printf("Digite o número total de territórios (máx %d): ", MAX_TERRITORIOS);
    scanf("%d", &totalTerritorios);

    if (totalTerritorios <= 0 || totalTerritorios > MAX_TERRITORIOS) {
        printf("Número inválido de territórios.\n");
        return 1;
    }

    // Aloca territórios dinamicamente
    Territorio* territorios = cadastrarTerritorios(totalTerritorios);

    // Aloca missões para cada jogador dinamicamente
    char* missoesJogadores[MAX_JOGADORES];
    for (int i = 0; i < MAX_JOGADORES; i++) {
        missoesJogadores[i] = (char*)malloc(100 * sizeof(char));
        atribuirMissao(missoesJogadores[i], missoesDisponiveis, TOTAL_MISSOES);
        printf("\nMissão do Jogador %d: %s\n", i + 1, missoesJogadores[i]);
    }

    int turno = 0;
    int vencedor = -1;

    int opcao;
    do {
        printf("\n=== TURNO DO JOGADOR %d ===\n", turno + 1);
        printf("1. Exibir territórios\n");
        printf("2. Realizar ataque\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                exibirTerritorios(territorios, totalTerritorios);
                break;
            case 2: {
                exibirTerritorios(territorios, totalTerritorios);
                int at, def;
                printf("Escolha o índice do território atacante: ");
                scanf("%d", &at);
                printf("Escolha o índice do território defensor: ");
                scanf("%d", &def);

                if (at < 0 || at >= totalTerritorios || def < 0 || def >= totalTerritorios) {
                    printf("Índices inválidos!\n");
                } else if (strcmp(territorios[at].cor, territorios[def].cor) == 0) {
                    printf("Você não pode atacar um território da mesma cor!\n");
                } else if (territorios[at].tropas <= 1) {
                    printf("O atacante precisa ter mais de 1 tropa para atacar!\n");
                } else {
                    atacar(&territorios[at], &territorios[def]);
                }

                // Verifica se o jogador atual completou a missão
                if (verificarMissao(missoesJogadores[turno], territorios, totalTerritorios)) {
                    vencedor = turno;
                    printf("\n🎉 JOGADOR %d CUMPRIU A MISSÃO: %s 🎉\n", turno + 1, missoesJogadores[turno]);
                    opcao = 0; // Termina o jogo
                } else {
                    turno = (turno + 1) % MAX_JOGADORES;
                }

                break;
            }
            case 0:
                liberarMemoria(territorios, missoesJogadores, MAX_JOGADORES);
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    if (vencedor != -1) {
        printf("\n🏆 Fim de jogo! Jogador %d venceu com sua missão!\n", vencedor + 1);
    }

    return 0;
}

// Função que aloca e cadastra territórios
Territorio* cadastrarTerritorios(int total) {
    Territorio* vetor = (Territorio*)calloc(total, sizeof(Territorio));

    printf("\n=== Cadastro de Territórios ===\n");

    for (int i = 0; i < total; i++) {
        printf("\nCadastro do território %d:\n", i);

        printf("Digite o nome do território: ");
        scanf(" %[^\n]", vetor[i].nome);  // Permite ler espaços

        printf("Digite a cor do exército: ");
        scanf("%s", vetor[i].cor);

        printf("Digite o número de tropas: ");
        scanf("%d", &vetor[i].tropas);
    }

    return vetor;
}

// Exibe todos os territórios
void exibirTerritorios(Territorio* territorios, int total) {
    printf("\n=== Territórios Cadastrados ===\n");
    for (int i = 0; i < total; i++) {
        printf("[%d] Nome: %s | Cor: %s | Tropas: %d\n",
               i, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
}

// Simula um ataque
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoA = rand() % 6 + 1;
    int dadoD = rand() % 6 + 1;

    printf("\nDados rolados => Atacante: %d | Defensor: %d\n", dadoA, dadoD);

    if (dadoA > dadoD) {
        printf("O atacante venceu!\n");

        // Transferência de cor
        strcpy(defensor->cor, atacante->cor);

        // Transferência de metade das tropas
        int tropasTransferidas = atacante->tropas / 2;
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;

    } else {
        printf("O defensor resistiu ao ataque.\n");
        atacante->tropas -= 1;
    }

    printf("\n--- Estado Atualizado ---\n");
    printf("Atacante: %s | Tropas: %d\n", atacante->nome, atacante->tropas);
    printf("Defensor: %s | Tropas: %d | Cor: %s\n", defensor->nome, defensor->tropas, defensor->cor);
}

// Sorteia e atribui missão ao jogador
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Lógica simplificada de verificação de missão
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    if (strcmp(missao, "Controlar pelo menos 4 territórios") == 0) {
        // Conta quantos territórios têm a mesma cor
        int contagem[10] = {0}; // até 10 cores diferentes hipotéticas
        for (int i = 0; i < tamanho; i++) {
            for (int j = 0; j < 10; j++) {
                if (contagem[j] == 0 || strcmp(mapa[i].cor, mapa[j].cor) == 0) {
                    contagem[j]++;
                    break;
                }
            }
        }

        for (int i = 0; i < 10; i++) {
            if (contagem[i] >= 4) return 1;
        }

    } else if (strcmp(missao, "Ter um território com mais de 10 tropas") == 
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.
    

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
